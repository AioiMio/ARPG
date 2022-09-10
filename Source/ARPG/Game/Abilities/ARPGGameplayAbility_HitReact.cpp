// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayAbility_HitReact.h"

UARPGGameplayAbility_HitReact::UARPGGameplayAbility_HitReact()
{
	// Cache tags
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));
}

void UARPGGameplayAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	switch (TriggerEventData->EventTag)
	{
	case HitDirectionLeftTag:
		StartSection = FName("L");
		break;
	case HitDirectionFrontTag:
		StartSection = FName("F");
		break;
	case HitDirectionRightTag:
		StartSection = FName("R");
		break;
	case HitDirectionBackTag:
		StartSection = FName("B");
		break;
	default: StartSection = FName("F");
	}
}
