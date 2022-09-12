// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCombatManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ARPG/Game/Abilities/ARPGGameplayEffect_HitReact.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

UARPGCombatManager::UARPGCombatManager()
{
	HitEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Hit"));
	TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
}

void UARPGCombatManager::ApplyDamage(float SkillMultiplier, EAttackHitType HitType) const
{
}

void UARPGCombatManager::SendDamageToActor(AActor* Target, EAttackHitType HitType)
{
	if (AARPGCharacter* Character = Cast<AARPGCharacter>(Target))
	{
		UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent();
		UAbilitySystemComponent* SourceASC = OwnerCharacter->GetAbilitySystemComponent();
		if (TargetASC && SourceASC)
		{
			TSubclassOf<UGameplayEffect> InEffect;
			
			switch (HitType)
			{
			case EAttackHitType::KnockBack: InEffect = KnockBackEffect;
				break;
			case EAttackHitType::KnockDown: InEffect = KnockDownEffect;
				break;
			case EAttackHitType::KnockUp: InEffect = KnockUpEffect;
				break;
			default: InEffect = nullptr;
			}
			
			if (InEffect)
			{
				SourceASC->ApplyHitReactEffectToTarget(InEffect.GetDefaultObject(), TargetASC);
			}
		}
	}
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
				FGameplayEventData HitEventData;
				HitEventData.Instigator = GetOwner();
				HitEventData.Target = HitCharacter;
				HitEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitEventTag, HitEventData);

				// FGameplayTag HitReactTag;
				// switch (CurrentAttackHitType)
				// {
				// case EAttackHitType::Normal: HitReactTag = FGameplayTag::RequestGameplayTag("Event.Hit.Normal");
				// 	break;
				// case EAttackHitType::Heavy: HitReactTag = FGameplayTag::RequestGameplayTag("Event.Hit.Heavy");
				// 	break;
				// case EAttackHitType::KnockDown: HitReactTag = FGameplayTag::RequestGameplayTag("Event.Hit.KnockDown");
				// 	break;
				// case EAttackHitType::KnockUp: HitReactTag = FGameplayTag::RequestGameplayTag("Event.Hit.KnockUp");
				// 	break;
				// case EAttackHitType::KnockBack: HitReactTag = FGameplayTag::RequestGameplayTag("Event.Hit.KnockBack");
				// 	break;
				// default: HitReactTag = FGameplayTag::RequestGameplayTag("Event.Hit.Normal");
				// }
				// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitReactTag, HitEventData);

				SendDamageToActor(HitCharacter, CurrentAttackHitType);
			}
		}
	}
}

void UARPGCombatManager::ApplyHitReact(EARPGHitReactDirection Direction)
{
}
