// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGGameplayAbility.h"
#include "ARPGGameplayAbility_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGGameplayAbility_HitReact : public UARPGGameplayAbility
{
	GENERATED_BODY()

public:
	UARPGGameplayAbility_HitReact();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

private:
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;

	FName StartSection;
};
