// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerState.h"

#include "ARPGPlayerController.h"
#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

AARPGPlayerState::AARPGPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSet = CreateDefaultSubobject<UARPGAttributeSet>(TEXT("AttributeSet"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 30.f;

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AARPGPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AARPGPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AARPGPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AARPGPlayerState::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegenRate();
}

float AARPGPlayerState::GetMana() const
{
	return AttributeSet->GetMana();
}

float AARPGPlayerState::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float AARPGPlayerState::GetManaRegenRate() const
{
	return AttributeSet->GetManaRegenRate();
}

float AARPGPlayerState::GetStamina() const
{
	return AttributeSet->GetStamina();
}

float AARPGPlayerState::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float AARPGPlayerState::GetStaminaRegenRate() const
{
	return AttributeSet->GetStaminaRegenRate();
}

float AARPGPlayerState::GetPosture() const
{
	return AttributeSet->GetPosture();
}

float AARPGPlayerState::GetMaxPosture() const
{
	return AttributeSet->GetMaxPosture();
}

float AARPGPlayerState::GetPostureRegenRate() const
{
	return AttributeSet->GetPostureRegenRate();
}

float AARPGPlayerState::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

int32 AARPGPlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetCharacterLevel();
}

int32 AARPGPlayerState::GetXP() const
{
	return AttributeSet->GetXP();
}

int32 AARPGPlayerState::GetXPBounty() const
{
	return AttributeSet->GetXPBounty();
}

int32 AARPGPlayerState::GetGold() const
{
	return AttributeSet->GetGold();
}

int32 AARPGPlayerState::GetGoldBounty() const
{
	return AttributeSet->GetGoldBounty();
}

void AARPGPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AARPGPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AARPGPlayerState::MaxHealthChanged);
		HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenRateAttribute()).AddUObject(this, &AARPGPlayerState::HealthRegenRateChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AARPGPlayerState::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).AddUObject(this, &AARPGPlayerState::MaxManaChanged);
		ManaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaRegenRateAttribute()).AddUObject(this, &AARPGPlayerState::ManaRegenRateChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AARPGPlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &AARPGPlayerState::MaxStaminaChanged);
		StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaRegenRateAttribute()).AddUObject(this, &AARPGPlayerState::StaminaRegenRateChanged);
		PostureChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetPostureAttribute()).AddUObject(this, &AARPGPlayerState::PostureChanged);
		MaxPostureChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxPostureAttribute()).AddUObject(this, &AARPGPlayerState::MaxPostureChanged);
		PostureRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetPostureRegenRateAttribute()).AddUObject(this, &AARPGPlayerState::PostureRegenRateChanged);
		MoveSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &AARPGPlayerState::MoveSpeedChanged);
		XPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetXPAttribute()).AddUObject(this, &AARPGPlayerState::XPChanged);
		GoldChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetGoldAttribute()).AddUObject(this, &AARPGPlayerState::GoldChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCharacterLevelAttribute()).AddUObject(this, &AARPGPlayerState::CharacterLevelChanged);

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AARPGPlayerState::StunTagChanged);
	}
}

void AARPGPlayerState::HealthChanged(const FOnAttributeChangeData & Data)
{
	float Health = Data.NewValue;
	float Damage = Health - Data.OldValue;

	// Update health bar
	AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		UARPGHealthBarWidget* HealthBar = PlayerCharacter->GetHealthBar();
		if (HealthBar)
		{
			HealthBar->SetHealthPercentage(Health / GetMaxHealth());
			HealthBar->ShowDamageNumber(Damage);
		}
	}
	
	// Update the HUD
	AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetOwner());
	if (PC)
	{
		UARPGHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetCurrentHealth(Health);
		}
	}
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
	
	// If the player died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->Die();
		}
	}
}

void AARPGPlayerState::MaxHealthChanged(const FOnAttributeChangeData & Data)
{
	float MaxHealth = Data.NewValue;

	// Update floating status bar
	AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		UARPGHealthBarWidget* HealthBar = PlayerCharacter->GetHealthBar();
		if (HealthBar)
		{
			HealthBar->SetHealthPercentage(GetHealth() / MaxHealth);
		}
	}

	// Update the HUD
	AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetOwner());
	if (PC)
	{
		UARPGHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxHealth(MaxHealth);
		}
	}
}

void AARPGPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float HealthRegenRate = Data.NewValue;
}

void AARPGPlayerState::ManaChanged(const FOnAttributeChangeData & Data)
{
	float Mana = Data.NewValue;
	
	// Update the HUD
	AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetOwner());
	if (PC)
	{
		UARPGHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetCurrentMana(Mana);
		}
	}
}

void AARPGPlayerState::MaxManaChanged(const FOnAttributeChangeData & Data)
{
	float MaxMana = Data.NewValue;
}

void AARPGPlayerState::ManaRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float ManaRegenRate = Data.NewValue;
}

void AARPGPlayerState::StaminaChanged(const FOnAttributeChangeData & Data)
{
	float Stamina = Data.NewValue;
	float Delta = Stamina - Data.OldValue;

	if (HasAuthority())
	{
		if (Stamina == GetMaxStamina())
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Ability.StaminaRegen.On"));
		}
	
		if (Delta < 0.f)
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Ability.StaminaRegen.On"));

			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AARPGPlayerState::StaminaRegenElapsed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaRegenDelay, Delegate, 1.5f, false);
		}

		if (Stamina == 0.f)
		{
			AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(GetPawn());
			if (PlayerCharacter)
			{
				PlayerCharacter->SprintStop();
			}
		}
	}
	
	// Update the HUD
	AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetOwner());
	if (PC)
	{
		UARPGHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetCurrentStamina(Stamina);
		}
	}
}

void AARPGPlayerState::StaminaRegenElapsed() const
{
	if (HasAuthority())
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Ability.StaminaRegen.On"));
	}
}

void AARPGPlayerState::MaxStaminaChanged(const FOnAttributeChangeData & Data)
{
	float MaxStamina = Data.NewValue;
}

void AARPGPlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float StaminaRegenRate = Data.NewValue;
}

void AARPGPlayerState::PostureChanged(const FOnAttributeChangeData& Data)
{
}

void AARPGPlayerState::MaxPostureChanged(const FOnAttributeChangeData& Data)
{
}

void AARPGPlayerState::PostureRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AARPGPlayerState::MoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	float MoveSpeed = Data.NewValue;

	AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
}

void AARPGPlayerState::XPChanged(const FOnAttributeChangeData & Data)
{
	float XP = Data.NewValue;
}

void AARPGPlayerState::GoldChanged(const FOnAttributeChangeData & Data)
{
	float Gold = Data.NewValue;
}

void AARPGPlayerState::CharacterLevelChanged(const FOnAttributeChangeData & Data)
{
	float CharacterLevel = Data.NewValue;
}

void AARPGPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
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
