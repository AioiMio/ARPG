// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGGameplayAbility.h"
#include "ARPGGameplayAbility_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGGameplayAbility_Dodge : public UARPGGameplayAbility
{
	GENERATED_BODY()

public:
	UARPGGameplayAbility_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
};
