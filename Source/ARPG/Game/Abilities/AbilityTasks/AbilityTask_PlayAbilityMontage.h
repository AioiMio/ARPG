// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilityTask_PlayAbilityMontage.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "PlayAbilityMontageAndWaitForEvent"))
class ARPG_API UAbilityTask_PlayAbilityMontage : public UAbilityTask_PlayMontageAndWaitForEvent
{
	GENERATED_BODY()

public:
	UAbilityTask_PlayAbilityMontage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;

	FGameplayTag EndAbilityTag;
};
