// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEnemyCharacter.h"

#include "Abilities/ComboGraphNativeAbility.h"
#include "ARPG/Game/AI/ARPGAIController.h"
#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/Components/InventoryComponent.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

AARPGEnemyCharacter::AARPGEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	HardRefAbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);

	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = HardRefAbilitySystemComponent;

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));

	// Create inventory component, and set it to be explicitly replicated
	HardRefInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HardRefInventoryComponent->SetIsReplicated(true);

	// Set our parent's TWeakObjectPtr
	InventoryComponent = HardRefInventoryComponent;

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	HardRefAttributeSet = CreateDefaultSubobject<UARPGAttributeSet>(TEXT("AttributeSet"));

	// Set our parent's TWeakObjectPtr
	AttributeSet = HardRefAttributeSet;

	CharacterName = FText::FromString("Enemy");
	HealthBarComponent->SetHiddenInGame(true);
}

void AARPGEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SensingComponent->OnSeePawn.AddDynamic(this, &AARPGEnemyCharacter::OnPawnSeen);
}

void AARPGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();
		InitializeHealthBar();

		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->
		                              GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).
		                              AddUObject(this, &AARPGEnemyCharacter::HealthChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->
									  GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).
									  AddUObject(this, &AARPGEnemyCharacter::StaminaChanged);
		PostureChangedDelegateHandle = AbilitySystemComponent->
									  GetGameplayAttributeValueChangeDelegate(AttributeSet->GetPostureAttribute()).
									  AddUObject(this, &AARPGEnemyCharacter::PostureChanged);

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")),
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AARPGEnemyCharacter::StunTagChanged);

		if (UARPGAbilitySystemComponent* ASC = GetARPGAbilitySystemComponent())
		{
			ASC->ReceivedDamage.AddDynamic(this, &AARPGEnemyCharacter::OnDamageReceived);
		}
	}
}

void AARPGEnemyCharacter::AddCharacterAbilities()
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
	// Grant ComboGraphNativeAbility for behavior tree
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UComboGraphNativeAbility::StaticClass()));

	AbilitySystemComponent->CharacterAbilitiesGiven = true;

	if (EquipmentManager)
	{
		EquipmentManager->AddEquipmentAbilitiesToOwner(EquipmentManager->GetCurrentRightHandWeapon());
		EquipmentManager->AddEquipmentAbilitiesToOwner(EquipmentManager->GetCurrentLeftHandWeapon());
	}
}

void AARPGEnemyCharacter::OnPawnSeen(APawn* Pawn)
{
	if (AARPGPlayerCharacter* Character = Cast<AARPGPlayerCharacter>(Pawn))
	{
		if (TargetManager->GetLockOnTarget() == nullptr)
		{
			TargetManager->SetLockOnTarget(Character);
			TargetManager->bIsLockingOn = true;

			AARPGAIController* AIC = Cast<AARPGAIController>(GetController());
			if (AIC)
			{
				UBlackboardComponent* BlackboardComponent = AIC->GetBlackboardComponent();
				BlackboardComponent->SetValueAsObject("TargetActor", Pawn);
			}
		}
	}
}

void AARPGEnemyCharacter::OnDamageReceived(UARPGAbilitySystemComponent* SourceASC,
	float UnmitigatedDamage,
	float MitigatedDamage)
{
	AARPGPlayerCharacter* Character = Cast<AARPGPlayerCharacter>(SourceASC->GetAvatarActor());
	if (Character && Character != TargetManager->GetLockOnTarget())
	{
		TargetManager->SetLockOnTarget(Character);
		TargetManager->bIsLockingOn = true;

		AARPGAIController* AIC = Cast<AARPGAIController>(GetController());
		if (AIC)
		{
			UBlackboardComponent* BlackboardComponent = AIC->GetBlackboardComponent();
			BlackboardComponent->SetValueAsObject("TargetActor", Character);
		}
	}
}

void AARPGEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	float Damage = Health - Data.OldValue;

	if (Health == GetMaxHealth())
	{
		HealthBarComponent->SetHiddenInGame(true);
	}
	else if (HealthBarComponent->bHiddenInGame == true)
	{
		HealthBarComponent->SetHiddenInGame(false);
	}
	
	// Update health bar
	if (HealthBar)
	{
		HealthBar->SetHealthPercentage(Health / GetMaxHealth());
		HealthBar->ShowDamageNumber(Damage);
	}

	// If the enemy died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		Die();
	}
}

void AARPGEnemyCharacter::StaminaChanged(const FOnAttributeChangeData& Data)
{
	float Stamina = Data.NewValue;
	float Delta = Stamina - Data.OldValue;

	if (HasAuthority())
	{
		if (Stamina == GetMaxStamina())
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(
				FGameplayTag::RequestGameplayTag("Ability.StaminaRegen.On"));
		}

		if (Delta < 0.f)
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(
				FGameplayTag::RequestGameplayTag("Ability.StaminaRegen.On"));

			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AARPGEnemyCharacter::StaminaRegenElapsed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaRegenDelay, Delegate, 1.5f, false);
		}

		if (Stamina == 0.f)
		{
			SprintStop();
		}
	}
}

void AARPGEnemyCharacter::StaminaRegenElapsed()
{
	if (HasAuthority())
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Ability.StaminaRegen.On"));
	}
}

void AARPGEnemyCharacter::PostureChanged(const FOnAttributeChangeData& Data)
{
	float Posture = Data.NewValue;
	float Delta = Posture - Data.OldValue;

	// Update health bar
	if (HealthBar)
	{
		HealthBar->SetPosturePercentage(GetPosture() / GetMaxPosture());
	}

	if (HasAuthority())
	{
		if (Posture == GetMaxPosture())
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(
				FGameplayTag::RequestGameplayTag("Ability.PostureRegen.On"));
		}

		if (Delta < 0.f)
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(
				FGameplayTag::RequestGameplayTag("Ability.PostureRegen.On"));

			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AARPGEnemyCharacter::PostureRegenElapsed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_PostureRegenDelay, Delegate, 3.f, false);
		}
	}

	if (IsAlive() && Posture <= 0.f)
	{
		Break();
		ResetPosture();
	}
}

void AARPGEnemyCharacter::PostureRegenElapsed()
{
	if (HasAuthority())
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Ability.PostureRegen.On"));
	}
}

void AARPGEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
