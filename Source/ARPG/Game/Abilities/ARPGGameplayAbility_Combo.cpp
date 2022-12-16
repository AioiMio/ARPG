// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayAbility_Combo.h"

#include "AbilityTasks/AbilityTask_PlayMontageAndWaitForEvent.h"

UARPGGameplayAbility_Combo::UARPGGameplayAbility_Combo()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
}

void UARPGGameplayAbility_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAnimMontage* MontageToPlay = ComboMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UAbilityTask_PlayMontageAndWaitForEvent* Task = UAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), PlayRate, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UARPGGameplayAbility_Combo::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UARPGGameplayAbility_Combo::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UARPGGameplayAbility_Combo::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UARPGGameplayAbility_Combo::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UARPGGameplayAbility_Combo::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UARPGGameplayAbility_Combo::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UARPGGameplayAbility_Combo::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UARPGGameplayAbility_Combo::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UARPGGameplayAbility_Combo::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}
