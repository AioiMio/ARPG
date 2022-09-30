// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayAbility_Gun.h"

#include "AbilityTasks/AbilityTask_PlayMontageAndWaitForEvent.h"
#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGCharacterMovementComponent.h"
#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/Equipments/Weapons/ARPGProjectile.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

UARPGGameplayAbility_Gun::UARPGGameplayAbility_Gun()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	Range = 2000.0f;
	Damage = 1.f;
}

void UARPGGameplayAbility_Gun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (UARPGCharacterMovementComponent* MovementComponent = Cast<UARPGCharacterMovementComponent>(ActorInfo->MovementComponent))
	{
		MovementComponent->StartWalking();
		if (AARPGCharacter* Character = Cast<AARPGCharacter>(GetAvatarActorFromActorInfo()))
		{
			if (Character->GetTargetManager()->bIsLockingOn)
			{
				MovementComponent->bOrientRotationToMovement = false;
				MovementComponent->bUseControllerDesiredRotation = true;
			}
		}
	}
	
	UAnimMontage* MontageToPlay = FireMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UAbilityTask_PlayMontageAndWaitForEvent* Task = UAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), PlayRate, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UARPGGameplayAbility_Gun::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UARPGGameplayAbility_Gun::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UARPGGameplayAbility_Gun::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UARPGGameplayAbility_Gun::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UARPGGameplayAbility_Gun::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UARPGGameplayAbility_Gun::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (UARPGCharacterMovementComponent* MovementComponent = Cast<UARPGCharacterMovementComponent>(ActorInfo->MovementComponent))
	{
		MovementComponent->StopWalking();
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UARPGGameplayAbility_Gun::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UARPGGameplayAbility_Gun::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UARPGGameplayAbility_Gun::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
	{
		if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
		{
			AARPGCharacter* SourceCharacter = Cast<AARPGCharacter>(GetAvatarActorFromActorInfo());
			if (!SourceCharacter)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}

			FVector Start = SourceCharacter->GetEquipmentManager()->GetCurrentLeftHandWeapon()->GetMesh()->GetSocketLocation(FName("Muzzle"));
			FVector End;
			if (AARPGCharacter* TargetCharacter = SourceCharacter->GetTargetManager()->GetLockOnTarget())
			{
				End = TargetCharacter->GetLockOnPointComponent()->GetComponentLocation();
			}
			else
			{
				End = Start + SourceCharacter->GetCapsuleComponent()->GetForwardVector() * Range;
			}
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

			FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		
			// Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
			DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);

			FTransform MuzzleTransform = SourceCharacter->GetEquipmentManager()->GetCurrentLeftHandWeapon()->GetMesh()->GetSocketTransform(FName("Muzzle"));
			MuzzleTransform.SetRotation(Rotation.Quaternion());
			MuzzleTransform.SetScale3D(FVector(1.0f));

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AARPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<AARPGProjectile>(ProjectileClass, MuzzleTransform, GetOwningActorFromActorInfo(),
				SourceCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
			Projectile->Range = Range;
			Projectile->FinishSpawning(MuzzleTransform);
		}
	}
}
