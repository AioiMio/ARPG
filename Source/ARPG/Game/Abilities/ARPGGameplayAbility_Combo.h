// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGGameplayAbility.h"
#include "ARPGGameplayAbility_Combo.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGGameplayAbility_Combo : public UARPGGameplayAbility
{
	GENERATED_BODY()

public:
	UARPGGameplayAbility_Combo();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* ComboMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	float PlayRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combo")
	TSubclassOf<UARPGGameplayAbility_Combo> NextCombo;
	
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
