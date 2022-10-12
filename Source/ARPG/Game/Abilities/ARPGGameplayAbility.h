// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ARPG/Game/Equipments/Weapons/ARPGProjectile.h"
#include "ARPG/Game/Types/Types.h"
#include "ARPGGameplayAbility.generated.h"

class AARPGCharacter;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UARPGGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(TSubclassOf<AARPGProjectile> ProjectileClass, APawn* Instigator, FTransform Transform, float Range, float Speed, const FGameplayEffectSpecHandle& EffectSpec);

	UFUNCTION(BlueprintCallable)
	void AddBlockAbilitiesWithTag(FGameplayTag Tag);
	
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EARPGAbilityInputID AbilityInputID = EARPGAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EARPGAbilityInputID AbilityID = EARPGAbilityInputID::None;

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffects;

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
