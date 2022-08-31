// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEnemyCharacter.h"

#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AARPGEnemyCharacter::AARPGEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	HardRefAbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);

	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	// HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = HardRefAbilitySystemComponent;

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	HardRefAttributeSet = CreateDefaultSubobject<UARPGAttributeSet>(TEXT("AttributeSet"));

	// Set our parent's TWeakObjectPtr
	AttributeSet = HardRefAttributeSet;
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

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")),
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AARPGEnemyCharacter::StunTagChanged);
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

	// If the minion died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		Die();
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
