// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEnemyCharacter.h"

#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Components/InventoryComponent.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AARPGEnemyCharacter::AARPGEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	HardRefAbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);

	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = HardRefAbilitySystemComponent;

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
	}
}

void AARPGEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	float Damage = Health - Data.OldValue;

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
