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
				// FGameplayEventData HitEventData;
				// HitEventData.Instigator = GetOwner();
				// HitEventData.Target = HitCharacter;
				// HitEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);
				// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitEventTag, HitEventData);

				SendHitEventToActor(HitCharacter, Hit, CurrentAttackHitType, CurrentSkillMultiplier);
			}
		}
	}
}

void UARPGCombatManager::SendHitEventToActor(AActor* Target, FHitResult Hit, EAttackHitType HitType, float Multiplier)
{
	if (AARPGCharacter* Character = Cast<AARPGCharacter>(Target))
	{
		UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent();
		UAbilitySystemComponent* SourceASC = OwnerCharacter->GetAbilitySystemComponent();
		if (TargetASC && SourceASC)
		{
			if (DamageEffectClass)
			{
				FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
				DamageEffectContext.AddInstigator(GetOwner(), GetOwner());
				DamageEffectContext.AddHitResult(Hit);
				FGameplayEffectSpecHandle DamageEffectSpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1, DamageEffectContext);
				DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), Multiplier);
				SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetASC);
			}
			
			TSubclassOf<UGameplayEffect> HitReactEffect;
			switch (HitType)
			{
			case EAttackHitType::KnockBack: HitReactEffect = KnockBackEffect;
				break;
			case EAttackHitType::KnockDown: HitReactEffect = KnockDownEffect;
				break;
			case EAttackHitType::KnockUp: HitReactEffect = KnockUpEffect;
				break;
			default: HitReactEffect = nullptr;
			}
			if (HitReactEffect)
			{
				FGameplayEffectContextHandle HitReactEffectContext = SourceASC->MakeEffectContext();
				HitReactEffectContext.AddInstigator(GetOwner(), GetOwner());
				HitReactEffectContext.AddHitResult(Hit);
				SourceASC->ApplyGameplayEffectToTarget(HitReactEffect.GetDefaultObject(), TargetASC, 1, HitReactEffectContext);
			}
		}
	}
}

void UARPGCombatManager::ApplyHitReact(EARPGHitReactDirection Direction)
{
}
