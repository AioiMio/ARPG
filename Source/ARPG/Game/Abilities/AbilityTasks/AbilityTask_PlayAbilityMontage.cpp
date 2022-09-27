// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_PlayAbilityMontage.h"

UAbilityTask_PlayAbilityMontage::UAbilityTask_PlayAbilityMontage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EndAbilityTag = FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility"));
}

void UAbilityTask_PlayAbilityMontage::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	Super::OnGameplayEvent(EventTag, Payload);
}
