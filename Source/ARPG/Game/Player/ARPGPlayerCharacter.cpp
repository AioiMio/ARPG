// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Components/ARPGCharacterMovementComponent.h"
#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Components/ARPGInteractComponent.h"
#include "ARPG/Game/Components/InventoryComponent.h"
#include "ARPG/Game/Core/ARPGGameModeBase.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


AARPGPlayerCharacter::AARPGPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Components
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->TargetOffset = FVector(0.0f, 0.0f, 120.0f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 15.0f;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(RootComponent);
	InteractSphere->SetSphereRadius(100.f);

	InteractComponent = CreateDefaultSubobject<UARPGInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetIsReplicated(true);

	CharacterName = FText::FromString("Player");
}

// Input
void AARPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetController());
	if (IsLocallyControlled() && EnhancedInputComponent && PC)
	{
		EnhancedInputComponent->BindAction(PC->MovementInput, ETriggerEvent::Started, this,
		                                   &AARPGPlayerCharacter::MovementInputBegin);
		EnhancedInputComponent->BindAction(PC->MovementInput, ETriggerEvent::Triggered, this,
		                                   &AARPGPlayerCharacter::MovementInput);
		EnhancedInputComponent->BindAction(PC->MovementInput, ETriggerEvent::Completed, this,
		                                   &AARPGPlayerCharacter::MovementInputEnd);
		EnhancedInputComponent->BindAction(PC->InteractInput, ETriggerEvent::Started, this,
		                                   &AARPGPlayerCharacter::Interact);
		// EnhancedInputComponent->BindAction(PC->CameraInput, ETriggerEvent::Triggered, this,
		// 								   &AARPGPlayerCharacter::CameraInput);
		// EnhancedInputComponent->BindAction(PC->CameraInput, ETriggerEvent::Completed, this,
		// 								   &AARPGPlayerCharacter::EndCameraInput);
		// EnhancedInputComponent->BindAction(PC->JumpInput, ETriggerEvent::Started, this,
		// 								   &ASCharacterBase::JumpStarted);
		// EnhancedInputComponent->BindAction(PC->DodgeInput, ETriggerEvent::Started, this,
		// 								   &ASCharacterBase::DodgeStarted);
		// EnhancedInputComponent->BindAction(PC->RightAttackInput, ETriggerEvent::Started, this,
		// 								   &AARPGPlayerCharacter::RightAttack);
		// EnhancedInputComponent->BindAction(PC->RightHeavyAttackInput, ETriggerEvent::Started, this,
		// 								   &AARPGPlayerCharacter::RightHeavyAttack);
		// EnhancedInputComponent->BindAction(PC->LockOnInput, ETriggerEvent::Started, this,
		// 								   &AARPGPlayerCharacter::LockOnPressed);
		// EnhancedInputComponent->BindAction(PC->ChangeTargetInput, ETriggerEvent::Started, this,
		// 								   &AARPGPlayerCharacter::ChangeTarget);
	}

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void AARPGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsLocallyControlled())
	{
		HealthBarComponent->SetHiddenInGame(true);
	}

	AARPGPlayerState* PS = GetPlayerState<AARPGPlayerState>();
	if (PS)
	{
		// Set Components on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		InventoryComponent = PS->GetInventoryComponent();

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSet for convenience attribute functions
		AttributeSet = PS->GetAttributeSet();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina/Posture to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());
		SetPosture(GetMaxPosture());

		// End respawn specific things

		AddStartupEffects();

		AddCharacterAbilities();

		AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		InitializeHealthBar();
	}
}

void AARPGPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AARPGPlayerState* PS = GetPlayerState<AARPGPlayerState>();
	if (PS)
	{
		// Set Components for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		InventoryComponent = PS->GetInventoryComponent();

		// Init ASC Actor Info for clients. Server will init its `ASC` when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSet = PS->GetAttributeSet();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		InitializeHealthBar();

		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());
		SetPosture(GetMaxPosture());
	}

	if (IsLocallyControlled())
	{
		ServerSetEmissive(true);
	}
}

void AARPGPlayerCharacter::Dying()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AARPGGameModeBase* GM = Cast<AARPGGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->PlayerCharacterDied(GetController());
		}
	}

	Super::Dying();
}

/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void AARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Only needed for Heroes placed in world and when the player is the Server.
	// On respawn, they are set up in PossessedBy.
	// When the player a client, the floating status bars are all set up in OnRep_PlayerState.
	InitializeHealthBar();
	if (IsLocallyControlled())
	{
		HealthBarComponent->SetHiddenInGame(true);
	}

	JoinWorld();
}

void AARPGPlayerCharacter::MovementInputBegin()
{
	AbilitySystemComponent->AddReplicatedGameplayTag(FGameplayTag::RequestGameplayTag("Input.Movement.Active"));
}

void AARPGPlayerCharacter::MovementInput(const FInputActionValue& InputActionValue)
{
	MovementInputVector = InputActionValue.Get<FInputActionValue::Axis2D>();
	const float Magnitude = InputActionValue.GetMagnitude();

	EGroundMovementType Temp;
	if (Magnitude <= 0.2f)
	{
		Temp = EGroundMovementType::Idle;
	}
	else if (Magnitude > 0.2f && Magnitude <= 0.6f)
	{
		Temp = EGroundMovementType::Walk;
	}
	else
	{
		Temp = EGroundMovementType::Run;
	}

	if (GroundMovement != Temp)
	{
		ServerSetGroundMovement(Temp);
	}

	if (Magnitude > 0.2f)
	{
		FVector WorldDirection = GetControlRotation().RotateVector(
			FVector(MovementInputVector.Y, MovementInputVector.X, 0.f));
		WorldDirection.Normalize();
		AddMovementInput(WorldDirection);
	}
}

void AARPGPlayerCharacter::MovementInputEnd()
{
	AbilitySystemComponent->RemoveReplicatedGameplayTag(FGameplayTag::RequestGameplayTag("Input.Movement.Active"));
	MovementInputVector = FVector2D::Zero();

	ServerSetGroundMovement(EGroundMovementType::Idle);
}

void AARPGPlayerCharacter::Interact()
{
	if (InteractComponent)
	{
		InteractComponent->Interact();
	}
}

void AARPGPlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(
			                                                              FString("ConfirmTarget"),
			                                                              FString("CancelTarget"),
			                                                              FString("EARPGAbilityInputID"),
			                                                              static_cast<int32>(
				                                                              EARPGAbilityInputID::Confirm),
			                                                              static_cast<int32>(
				                                                              EARPGAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AARPGPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPGPlayerCharacter, MovementInputVector);
}
