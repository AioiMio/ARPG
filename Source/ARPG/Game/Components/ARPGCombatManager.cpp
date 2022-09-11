// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCombatManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

UARPGCombatManager::UARPGCombatManager()
{
	HitEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Hit"));
	TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
}

void UARPGCombatManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPGCharacter>(GetOuter());
	OnAttackHitEvent.AddDynamic(this, &UARPGCombatManager::OnAttackHit);
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->ShowHitReact.AddDynamic(this, &UARPGCombatManager::ApplyHitReact);
	}
}

void UARPGCombatManager::OnAttackHit(FHitResult Hit, UPrimitiveComponent* Mesh)
{
	if (OwnerCharacter->HasAuthority())
	{
		if (!IgnoredActors.Contains(Hit.GetActor()))
		{
			IgnoredActors.AddUnique(Hit.GetActor());
			if (AARPGCharacter* HitCharacter = Cast<AARPGCharacter>(Hit.GetActor()))
			{
				FGameplayEventData GameplayEventData;
				GameplayEventData.Instigator = GetOwner();
				GameplayEventData.Target = HitCharacter;
				GameplayEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);

				FGameplayTag EventTag;
				switch (CurrentAttackHitType)
				{
				case EAttackHitType::Normal: EventTag = FGameplayTag::RequestGameplayTag("Event.Hit.Normal");
					break;
				case EAttackHitType::Heavy: EventTag = FGameplayTag::RequestGameplayTag("Event.Hit.Heavy");
					break;
				case EAttackHitType::KnockDown: EventTag = FGameplayTag::RequestGameplayTag("Event.Hit.KnockDown");
					break;
				case EAttackHitType::KnockUp: EventTag = FGameplayTag::RequestGameplayTag("Event.Hit.KnockUp");
					break;
				case EAttackHitType::KnockBack: EventTag = FGameplayTag::RequestGameplayTag("Event.Hit.KnockBack");
					break;
				default: EventTag = FGameplayTag::RequestGameplayTag("Event.Hit.Normal");
				}
				
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitEventTag, GameplayEventData);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EventTag, GameplayEventData);
			}
		}
	}
}

void UARPGCombatManager::ApplyHitReact(EARPGHitReactDirection Direction)
{
}
