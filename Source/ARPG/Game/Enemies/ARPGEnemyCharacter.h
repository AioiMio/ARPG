// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPGEnemyCharacter.generated.h"

class UPawnSensingComponent;

/**
 * 
 */
UCLASS()
class ARPG_API AARPGEnemyCharacter : public AARPGCharacter
{
	GENERATED_BODY()

public:
	AARPGEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite)
	bool bHit;

protected:
	virtual void BeginPlay() override;
	virtual void AddCharacterAbilities() override;
	virtual void PostInitializeComponents() override;
	
	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	UARPGAbilitySystemComponent* HardRefAbilitySystemComponent;

	// Actual hard pointer to InventoryComponent
	UPROPERTY()
	UInventoryComponent* HardRefInventoryComponent;

	// Actual hard pointer to AttributeSet
	UPROPERTY()
	UARPGAttributeSet* HardRefAttributeSet;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* SensingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> ComboGraphNativeAbility;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle PostureChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void PostureChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	FTimerHandle TimerHandle_StaminaRegenDelay;
	FTimerHandle TimerHandle_PostureRegenDelay;
	FTimerHandle TimerHandle_RemoveTargetDelay;
	FTimerHandle TimerHandle_RemoveHitStateDelay;
	
	void StaminaRegenElapsed();
	void PostureRegenElapsed();
	void RemoveTargetElapsed();
	void RemoveHitStateElapsed();

	UFUNCTION()
	void UpdateEnmity();
	
	void SetTarget(APawn* Pawn);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnDamageReceived(UARPGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
