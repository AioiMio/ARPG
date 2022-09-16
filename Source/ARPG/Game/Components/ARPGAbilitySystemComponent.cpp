// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAbilitySystemComponent.h"

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

void UARPGAbilitySystemComponent::ServerRemoveReplicatedGameplayTag_Implementation(FGameplayTag Tag)
{
	MulticastRemoveReplicatedGameplayTag(Tag);
}

void UARPGAbilitySystemComponent::MulticastRemoveReplicatedGameplayTag_Implementation(FGameplayTag Tag)
{
	RemoveLooseGameplayTag(Tag);
}