// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerCharacter.h"

#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Core/ARPGGameModeBase.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

	CharacterName = FText::FromString("Player");
}

void AARPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

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

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

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
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its `ASC` when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

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
