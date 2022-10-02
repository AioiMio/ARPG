// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayAbility.h"

#include "AbilitySystemComponent.h"

UARPGGameplayAbility::UARPGGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Break")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Lying")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void UARPGGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GameplayEffects.Num() > 0)
	{
		for (const TSubclassOf<UGameplayEffect>& Effect : GameplayEffects)
		{
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(Effect);
			ActiveGameplayEffects.Add(ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpecHandle));
		}
	}
}

void UARPGGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActiveGameplayEffects.Num() > 0)
	{
		for (const FActiveGameplayEffectHandle& ActiveGameplayEffect : ActiveGameplayEffects)
		{
			GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveGameplayEffect);
		}
	}
	ActiveGameplayEffects.Empty();
}

void UARPGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
