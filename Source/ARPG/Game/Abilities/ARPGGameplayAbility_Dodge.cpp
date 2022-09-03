// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayAbility_Dodge.h"

#include "ARPG/Game/Core/ARPGCharacter.h"

UARPGGameplayAbility_Dodge::UARPGGameplayAbility_Dodge()
{
}

void UARPGGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		AARPGCharacter* Character = Cast<AARPGCharacter>(GetAvatarActorFromActorInfo());
	}
}

void UARPGGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
