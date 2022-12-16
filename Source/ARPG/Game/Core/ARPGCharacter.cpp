// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ARPG/Game/Abilities/ARPGGameplayAbility.h"
#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Components/ARPGCharacterMovementComponent.h"
#include "ARPG/Game/Components/ARPGCombatManager.h"
#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Components/ARPGMotionWarpingComponent.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AARPGCharacter::AARPGCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UARPGCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	EquipmentManager = CreateDefaultSubobject<UARPGEquipmentManager>(FName("EquipmentManager"));
	EquipmentManager->SetIsReplicated(true);
	CombatManager = CreateDefaultSubobject<UARPGCombatManager>(FName("CombatManager"));
	TargetManager = CreateDefaultSubobject<UARPGTargetManager>(FName("TargetManager"));
	TargetManager->SetIsReplicated(true);
	MotionWarpingComponent = CreateDefaultSubobject<UARPGMotionWarpingComponent>(FName("MotionWarping"));
	// MotionWarpingComponent->SetIsReplicated(true);

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("HealthBar"));
	HealthBarComponent->SetupAttachment(GetMesh(), FName("head"));
	HealthBarComponent->SetRelativeLocation(FVector(25.f, 0.f, 0.f));
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarComponent->SetDrawAtDesiredSize(true);
	HealthBarComponent->SetPivot(FVector2D(0.5f, 1.f));
	HealthBarComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LockOnPointComponent = CreateDefaultSubobject<UWidgetComponent>(FName("LockOnPoint"));
	LockOnPointComponent->SetupAttachment(GetMesh(), FName("spine_02"));
	LockOnPointComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnPointComponent->SetDrawAtDesiredSize(true);
	LockOnPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LockOnPointComponent->SetHiddenInGame(true);

	// Setup Collisions
	GetCapsuleComponent()->SetCollisionProfileName(FName("PawnCapsule"));
	GetMesh()->SetCollisionProfileName(FName("HitTraceMesh"));
	GetMesh()->SetGenerateOverlapEvents(true);

	bAlwaysRelevant = true;

	// Cache tags
	HitEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Hit"));
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
	FallingTag = FGameplayTag::RequestGameplayTag(FName("State.Falling"));
	JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Common.Jump"));
	DodgeTag = FGameplayTag::RequestGameplayTag(FName("Ability.Common.Dodge"));
	SprintTag = FGameplayTag::RequestGameplayTag(FName("Ability.Common.Sprint"));
	RightHandAttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Melee.Combo"));
	RightHandHeavyAttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Melee.RightHeavy"));
	LeftHandHeavyAttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Melee.LeftHeavy"));
	GunAttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Gun"));
	VisceralAttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Common.VisceralAttack"));

	// Settings
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 4.f;
	GetCharacterMovement()->JumpZVelocity = 1200.f;
	GetCharacterMovement()->AirControl = 0.4f;
	GetCharacterMovement()->RotationRate.Yaw = 1200.f;
	GetCapsuleComponent()->SetCapsuleRadius(40.f);
}

void AARPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}

void AARPGCharacter::ServerSetEmissive_Implementation(bool bInEmissive)
{
	bEmissive = bInEmissive;
	GetMesh()->SetScalarParameterValueOnMaterials(FName("EmissivePower"), bEmissive ? 10.f : 0.f);
}

void AARPGCharacter::OnRep_bEmissive(const bool& bOldEmissive)
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("EmissivePower"), bEmissive ? 10.f : 0.f);
}

void AARPGCharacter::OnRep_GroundMovement(EGroundMovementType OldGroundMovement)
{
}

void AARPGCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if (PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			this, FGameplayTag::RequestGameplayTag("Event.Movement.Land"), FGameplayEventData());

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "LandElapsed");
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_LandDelay, Delegate, 0.05f, false);
	}
}

void AARPGCharacter::LandElapsed()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "RemoveFallingTagElapsed");
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_FallingTagRemoveDelay, Delegate, 0.05f, false);
}

void AARPGCharacter::RemoveFallingTagElapsed()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FallingTag));
	}
}

UAbilitySystemComponent* AARPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UInventoryComponent* AARPGCharacter::GetInventoryComponent() const
{
	return InventoryComponent.Get();
}

bool AARPGCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AARPGCharacter::GetAbilityLevel(EARPGAbilityInputID AbilityID) const
{
	return 1;
}

void AARPGCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->
		CharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void AARPGCharacter::OnHitEffect()
{
}

EARPGHitReactDirection AARPGCharacter::GetHitReactDirection(const FVector& ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EARPGHitReactDirection::Front;
		}
		else
		{
			return EARPGHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EARPGHitReactDirection::Right;
		}
		else
		{
			return EARPGHitReactDirection::Left;
		}
	}

	return EARPGHitReactDirection::Front;
}

void AARPGCharacter::PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser)
{
	if (IsAlive())
	{
		FGameplayEventData HitReactEventData;
		HitReactEventData.EventTag = HitDirection;
		HitReactEventData.Instigator = DamageCauser;
		HitReactEventData.Target = this;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitDirection, HitReactEventData);

		if (HitDirection == HitDirectionLeftTag)
		{
			ShowHitReact.Broadcast(EARPGHitReactDirection::Left);
		}
		else if (HitDirection == HitDirectionFrontTag)
		{
			ShowHitReact.Broadcast(EARPGHitReactDirection::Front);
		}
		else if (HitDirection == HitDirectionRightTag)
		{
			ShowHitReact.Broadcast(EARPGHitReactDirection::Right);
		}
		else if (HitDirection == HitDirectionBackTag)
		{
			ShowHitReact.Broadcast(EARPGHitReactDirection::Back);
		}
	}
}

bool AARPGCharacter::PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser)
{
	return true;
}

void AARPGCharacter::SetLockOnPointHiddenInGame(bool bInHidden)
{
	LockOnPointComponent->bHiddenInGame = bInHidden;
}

void AARPGCharacter::JumpAction()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(JumpTag));
}

void AARPGCharacter::DodgeAction()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(DodgeTag));
}

void AARPGCharacter::SprintStart()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(SprintTag));
}

void AARPGCharacter::SprintStop()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this, FGameplayTag::RequestGameplayTag("Event.Movement.EndSprint"), FGameplayEventData());
}

void AARPGCharacter::RightHandAttackAction()
{
	if (!CombatManager->TryVisceralAttack())
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(RightHandAttackTag));
	}
}

void AARPGCharacter::RightHandHeavyAttackAction()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(RightHandHeavyAttackTag));
}

void AARPGCharacter::LeftHandHeavyAttackAction()
{
	if (!AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(GunAttackTag)))
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(LeftHandHeavyAttackTag));
	}
}

int32 AARPGCharacter::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
	{
		return static_cast<int32>(AttributeSet->GetCharacterLevel());
	}

	return 0;
}

float AARPGCharacter::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AARPGCharacter::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AARPGCharacter::GetMana() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMana();
	}

	return 0.0f;
}

float AARPGCharacter::GetMaxMana() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxMana();
	}

	return 0.0f;
}

float AARPGCharacter::GetStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina();
	}

	return 0.0f;
}

float AARPGCharacter::GetMaxStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float AARPGCharacter::GetPosture() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetPosture();
	}

	return 0.0f;
}

float AARPGCharacter::GetMaxPosture() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxPosture();
	}

	return 0.0f;
}

float AARPGCharacter::GetMoveSpeed() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float AARPGCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue();
	}

	return 0.0f;
}

// Movement
void AARPGCharacter::SetGroundMovement(EGroundMovementType NewGroundMovement)
{
	if (NewGroundMovement == EGroundMovementType::Walk)
	{
		if (UARPGCharacterMovementComponent* CM = Cast<UARPGCharacterMovementComponent>(GetCharacterMovement()))
		{
			CM->StartWalking();
		}
	}
	else if (GroundMovement == EGroundMovementType::Walk)
	{
		if (UARPGCharacterMovementComponent* CM = Cast<UARPGCharacterMovementComponent>(GetCharacterMovement()))
		{
			CM->StopWalking();
		}
	}

	GroundMovement = NewGroundMovement;
}

void AARPGCharacter::ServerSetGroundMovement_Implementation(EGroundMovementType NewGroundMovement)
{
	SetGroundMovement(NewGroundMovement);
}

// Run on Server and all clients
void AARPGCharacter::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	HealthBarComponent->SetHiddenInGame(true);
	LockOnPointComponent->SetHiddenInGame(true);
	if (AARPGCharacter* Target = TargetManager->GetLockOnTarget())
	{
		Target->SetLockOnPointHiddenInGame(true);
	}
	TargetManager->SetLockOnTarget(nullptr);
	TargetManager->bIsLockingOn = false;

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if (DeathMontage && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		Dying();
	}
}

void AARPGCharacter::Break()
{
	if (AbilitySystemComponent.IsValid())
	{
		if (BreakEffect)
		{
			FGameplayEffectSpecHandle BreakEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
				BreakEffect, 1.f, FGameplayEffectContextHandle());
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*BreakEffectSpecHandle.Data.Get());
		}
	}
}

void AARPGCharacter::Dying()
{
	MulticastLeaveWorld();
	EquipmentManager->DissolveAllWeapons();
}

void AARPGCharacter::FinishDying()
{
	Destroy();
}

void AARPGCharacter::PreRestart()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
	}

	TargetManager->SetLockOnTarget(nullptr);
	Destroy();
}

void AARPGCharacter::ResetPosture(float ResetDelay)
{
	if (ResetDelay > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ResetPostureElapsed");
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetPostureDelay, Delegate, ResetDelay, false);
	}
	else
	{
		SetPosture(GetMaxPosture());
	}
}

void AARPGCharacter::ResetPostureElapsed()
{
	SetPosture(GetMaxPosture());
}

void AARPGCharacter::Destroyed()
{
	EquipmentManager->DestroyAllWeapons();
	Super::Destroyed();
}

void AARPGCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->
		CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UARPGGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
			                     static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;

	if (EquipmentManager)
	{
		EquipmentManager->AddEquipmentAbilitiesToOwner(EquipmentManager->GetCurrentRightHandWeapon());
		EquipmentManager->AddEquipmentAbilitiesToOwner(EquipmentManager->GetCurrentLeftHandWeapon());
	}
}

void AARPGCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."),
		       *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
			*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AARPGCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->
		StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
			GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AARPGCharacter::InitializeHealthBar()
{
	// Only create once
	if (!HealthBarComponent || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Setup HealthBar UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->IsLocalPlayerController())
	{
		if (HealthBarClass)
		{
			HealthBar = CreateWidget<UARPGHealthBarWidget>(PC, HealthBarClass);
			if (HealthBarClass && HealthBarComponent)
			{
				HealthBarComponent->SetWidget(HealthBar);

				// Setup the floating status bar
				HealthBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
				HealthBar->SetPosturePercentage(GetPosture() / GetMaxPosture());

				HealthBar->SetCharacterName(CharacterName);
			}
		}
	}
}

void AARPGCharacter::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void AARPGCharacter::SetMana(float Mana)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetMana(Mana);
	}
}

void AARPGCharacter::SetStamina(float Stamina)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetStamina(Stamina);
	}
}

void AARPGCharacter::SetPosture(float Posture)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetPosture(Posture);
	}
}

void AARPGCharacter::MulticastLeaveWorld_Implementation()
{
	LeaveWorld();
}

void AARPGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPGCharacter, bEmissive);
	DOREPLIFETIME(AARPGCharacter, GroundMovement);
}
