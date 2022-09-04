// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "ARPGPlayerState.generated.h"

class UARPGAbilitySystemComponent;
class UARPGAttributeSet;

/**
 * 
 */
UCLASS()
class ARPG_API AARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AARPGPlayerState();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UARPGAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState")
	bool IsAlive() const;

	/**
	* Getters for attributes from ARPGAttributeSet. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetManaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetPosture() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetMaxPosture() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetPostureRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	int32 GetXP() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	int32 GetXPBounty() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "ARPGPlayerState|Attributes")
	int32 GetGoldBounty() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UARPGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UARPGAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	FTimerHandle TimerHandle_StaminaRegenDelay;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle ManaRegenRateChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;
	FDelegateHandle PostureChangedDelegateHandle;
	FDelegateHandle MaxPostureChangedDelegateHandle;
	FDelegateHandle PostureRegenRateChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;
	FDelegateHandle XPChangedDelegateHandle;
	FDelegateHandle GoldChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void ManaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void PostureChanged(const FOnAttributeChangeData& Data);
	virtual void MaxPostureChanged(const FOnAttributeChangeData& Data);
	virtual void PostureRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);
	virtual void XPChanged(const FOnAttributeChangeData& Data);
	virtual void GoldChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void StaminaRegenElapsed() const;
};
