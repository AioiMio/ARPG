// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ARPGAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UARPGAbilitySystemComponent*, SourceASC, float,
                                               UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS()
class ARPG_API UARPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from ARPGDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UARPGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	void AddReplicatedGameplayTag(FGameplayTag Tag);
	UFUNCTION(Server, Reliable)
	void ServerAddReplicatedGameplayTag(FGameplayTag Tag);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddReplicatedGameplayTag(FGameplayTag Tag);

	void RemoveReplicatedGameplayTag(FGameplayTag Tag);
	UFUNCTION(Server, Reliable)
	void ServerRemoveReplicatedGameplayTag(FGameplayTag Tag);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRemoveReplicatedGameplayTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag,
	                             const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
};
