// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

void UARPGAbilitySystemComponent::ReceiveDamage(UARPGAbilitySystemComponent* SourceASC,
                                                float UnmitigatedDamage,
                                                float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UARPGAbilitySystemComponent::AddReplicatedGameplayTag(FGameplayTag Tag)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		MulticastAddReplicatedGameplayTag(Tag);
	}
	else
	{
		ServerAddReplicatedGameplayTag(Tag);
	}
}

void UARPGAbilitySystemComponent::ServerAddReplicatedGameplayTag_Implementation(FGameplayTag Tag)
{
	MulticastAddReplicatedGameplayTag(Tag);
}

void UARPGAbilitySystemComponent::MulticastAddReplicatedGameplayTag_Implementation(FGameplayTag Tag)
{
	AddLooseGameplayTag(Tag);
}

void UARPGAbilitySystemComponent::RemoveReplicatedGameplayTag(FGameplayTag Tag)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		MulticastRemoveReplicatedGameplayTag(Tag);
	}
	else
	{
		ServerRemoveReplicatedGameplayTag(Tag);
	}
}

void UARPGAbilitySystemComponent::SendGameplayEventToActor(AActor* Actor,
                                                           FGameplayTag EventTag,
                                                           FGameplayEventData Payload)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		ServerSendGameplayEventToActor(Actor, EventTag, Payload);
	}
	else
	{
		AARPGCharacter* InCharacter = Cast<AARPGCharacter>(Actor);
		if (!InCharacter->IsLocallyControlled())
		{
			InCharacter->GetARPGAbilitySystemComponent()->ClientSendGameplayEventToActor(Actor, EventTag, Payload);
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
	}
}

void UARPGAbilitySystemComponent::ServerSendGameplayEventToActor_Implementation(AActor* Actor,
	FGameplayTag EventTag,
	FGameplayEventData Payload)
{
	SendGameplayEventToActor(Actor, EventTag, Payload);
}

void UARPGAbilitySystemComponent::ClientSendGameplayEventToActor_Implementation(AActor* Actor,
	FGameplayTag EventTag,
	FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
}

void UARPGAbilitySystemComponent::ServerApplyGameplayEffectToSelf_Implementation(
	TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (IsValid(GameplayEffect))
	{
		FGameplayEffectContextHandle EffectContext;
		FGameplayEffectSpecHandle GameplayEffectSpec = MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data);
	}
}

void UARPGAbilitySystemComponent::ServerRemoveGameplayEffectFromSelf_Implementation(
	TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (IsValid(GameplayEffect))
	{
		RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, this);
	}
}

void UARPGAbilitySystemComponent::ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag,
                                                          const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Executed, GameplayCueParameters);
}

void UARPGAbilitySystemComponent::AddGameplayCueLocal(const FGameplayTag GameplayCueTag,
                                                      const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::OnActive, GameplayCueParameters);
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::WhileActive, GameplayCueParameters);
}

void UARPGAbilitySystemComponent::RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag,
                                                         const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Removed, GameplayCueParameters);
}

void UARPGAbilitySystemComponent::ServerRemoveReplicatedGameplayTag_Implementation(FGameplayTag Tag)
{
	MulticastRemoveReplicatedGameplayTag(Tag);
}

void UARPGAbilitySystemComponent::MulticastRemoveReplicatedGameplayTag_Implementation(FGameplayTag Tag)
{
	RemoveLooseGameplayTag(Tag);
}
