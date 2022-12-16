// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

UARPGGameplayAbility::UARPGGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Break")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Lying")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Rolling")));
}

void UARPGGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		if (TriggerEventData)
		{
			// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
			K2_ActivateAbilityFromEvent(*TriggerEventData);
		}
		else
		{
			UE_LOG(LogAbilitySystem, Warning,
			       TEXT(
				       "Ability %s expects event data but none is being supplied. Use Activate Ability instead of Activate Ability From Event."
			       ), *GetName());
			bool bReplicateEndAbility = false;
			bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}
	else if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (GameplayEffects.Num() > 0)
	{
		for (const TSubclassOf<UGameplayEffect>& Effect : GameplayEffects)
		{
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(Effect);
			ActiveGameplayEffects.Add(
				ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpecHandle));
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

void UARPGGameplayAbility::SpawnProjectile(TSubclassOf<AARPGProjectile> ProjectileClass,
                                           APawn* Instigator,
                                           AActor* Target,
                                           FTransform Transform,
                                           float Range,
                                           float Speed,
                                           const FGameplayEffectSpecHandle& EffectSpec)
{
	AARPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<AARPGProjectile>(
		ProjectileClass, Transform, GetOwningActorFromActorInfo(),
		Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->DamageEffectSpecHandle = EffectSpec;
	Projectile->Range = Range;
	Projectile->Speed = Speed;
	Projectile->Target = Target;
	Projectile->FinishSpawning(Transform);
}

void UARPGGameplayAbility::AddBlockAbilitiesWithTag(FGameplayTag Tag)
{
	BlockAbilitiesWithTag.AddTag(Tag);
}

void UARPGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
