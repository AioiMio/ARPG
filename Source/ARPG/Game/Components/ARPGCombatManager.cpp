// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCombatManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ARPGAbilitySystemComponent.h"
#include "ARPGTargetManager.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

FARPGDamageData::FARPGDamageData()
{
	DamageMultipliers.Empty();
	PostureDamage = 0.f;
	ImpactLevel = -1;
	AttackType = EAttackType::None;
	HitType = EHitReactType::None;
}

FARPGDamageData::FARPGDamageData(EAttackType InAttackType,
	EHitReactType InHitType,
	TMap<EDamageType, float> InDamageMultipliers,
	float InPostureDamage,
	int32 InImpactLevel)
{
	AttackType = InAttackType;
	HitType = InHitType;
	DamageMultipliers = InDamageMultipliers;
	PostureDamage = InPostureDamage;
	ImpactLevel = InImpactLevel;
}

bool FARPGDamageData::IsValid() const
{
	return AttackType != EAttackType::None;
}

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
	OnStartAttack.AddDynamic(this, &UARPGCombatManager::OnAttackStart);
	OnEndAttack.AddDynamic(this, &UARPGCombatManager::OnAttackEnd);

	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->ShowHitReact.AddDynamic(this, &UARPGCombatManager::ApplyHitReact);
	}
}

void UARPGCombatManager::OnAttackStart()
{
	HitActors.Empty();
}

void UARPGCombatManager::OnAttackEnd()
{
}

void UARPGCombatManager::OnAttackHit(FHitResult Hit, UPrimitiveComponent* Mesh)
{
	if (OwnerCharacter->HasAuthority())
	{
		if (!IgnoredActors.Contains(Hit.GetActor()))
		{
			IgnoredActors.Add(Hit.GetActor());
			AARPGCharacter* HitCharacter = Cast<AARPGCharacter>(Hit.GetActor());
			if (HitCharacter && HitCharacter->IsAlive() && HitCharacter->GetCombatManager()->GetCharacterCamp() != CharacterCamp)
			{
				if (HitCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(
					FGameplayTag::RequestGameplayTag(FName("State.Immunity"))))
				{
					return;
				}

				HitEvent(HitCharacter, Hit, CurrentAttackType, CurrentHitReactType, CurrentSkillMultiplier);
			}
		}
	}
}

void UARPGCombatManager::ServerHitEvent_Implementation(AActor* Target,
                                                       FHitResult Hit,
                                                       EAttackType AttackType,
                                                       EHitReactType HitReactType,
                                                       float Multiplier)
{
	HitEvent(Target, Hit, AttackType, HitReactType, Multiplier);
}

void UARPGCombatManager::HitEvent(AActor* Target,
                                  FHitResult Hit,
                                  EAttackType AttackType,
                                  EHitReactType HitReactType,
                                  float Multiplier)
{
	FGameplayEventData HitEventData;
	HitEventData.Instigator = GetOwner();
	HitEventData.Target = Target;
	HitEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitEventTag, HitEventData);

	SendHitEventToActor(Target, Hit, AttackType, HitReactType, Multiplier);
}

void UARPGCombatManager::SendHitEventToActor(AActor* Target,
                                             FHitResult Hit,
                                             EAttackType AttackType,
                                             EHitReactType HitReactType,
                                             float Multiplier)
{
	if (AARPGCharacter* Character = Cast<AARPGCharacter>(Target))
	{
		UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent();
		UAbilitySystemComponent* SourceASC = OwnerCharacter->GetAbilitySystemComponent();
		if (TargetASC && SourceASC)
		{
			FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
			DamageEffectContext.AddInstigator(GetOwner(), GetOwner());
			DamageEffectContext.AddHitResult(Hit);

			TSubclassOf<UGameplayEffect> DamageEffect;
			if (AttackType == EAttackType::Charge)
			{
				DamageEffect = ChargeDamageEffectClass;
			}
			else
			{
				DamageEffect = DamageEffectClass;
			}
			FGameplayEffectSpecHandle DamageEffectSpecHandle = TargetASC->MakeOutgoingSpec(
				DamageEffect, 1, DamageEffectContext);
			DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"),
			                                                     Multiplier);
			SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetASC);

			// HitReact
			TSubclassOf<UGameplayEffect> HitReactEffect;
			switch (HitReactType)
			{
			case EHitReactType::KnockBack: HitReactEffect = KnockBackEffect;
				break;
			case EHitReactType::KnockDown: HitReactEffect = KnockDownEffect;
				break;
			case EHitReactType::KnockUp: HitReactEffect = KnockUpEffect;
				break;
			default: HitReactEffect = nullptr;
			}
			if (HitReactEffect)
			{
				FGameplayEffectContextHandle HitReactEffectContext = SourceASC->MakeEffectContext();
				HitReactEffectContext.AddInstigator(GetOwner(), GetOwner());
				HitReactEffectContext.AddHitResult(Hit);
				SourceASC->ApplyGameplayEffectToTarget(HitReactEffect.GetDefaultObject(), TargetASC, 1,
				                                       HitReactEffectContext);
			}
		}
	}
}

void UARPGCombatManager::SendGameplayEventToOwner(FGameplayTag EventTag, FGameplayEventData Payload)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s, Owner: %s"), *GetNameSafe(GetOwner()),
		       *GetNameSafe(GetOwner()->GetOwner()));
		ServerSendGameplayEventToOwner(EventTag, Payload);
	}
	else
	{
		if (!OwnerCharacter->IsLocallyControlled())
		{
			ClientSendGameplayEventToOwner(EventTag, Payload);
		}
		OwnerCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(EventTag, &Payload);
	}
}

void UARPGCombatManager::ServerSendGameplayEventToOwner_Implementation(FGameplayTag EventTag,
                                                                       FGameplayEventData Payload)
{
	SendGameplayEventToOwner(EventTag, Payload);
}

void UARPGCombatManager::ClientSendGameplayEventToOwner_Implementation(FGameplayTag EventTag,
                                                                       FGameplayEventData Payload)
{
	OwnerCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(EventTag, &Payload);
}

bool UARPGCombatManager::TryVisceralAttack(bool bUseLockOnTarget)
{
	if (bUseLockOnTarget)
	{
		return TryExecuteVisceralAttackToTarget(OwnerCharacter->GetTargetManager()->GetLockOnTarget());
	}

	float Range = 50.f;
	float Radius = 35.f;
	FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * Radius;
	FVector End = Start + OwnerCharacter->GetActorForwardVector() * Range;
	TArray<FHitResult> HitResults;
	TArray<AActor*> InIgnoreActors;
	InIgnoreActors.Add(OwnerCharacter.Get());
	UKismetSystemLibrary::SphereTraceMulti(this, Start, End, Radius, TraceChannel, false, InIgnoreActors,
	                                       EDrawDebugTrace::None, HitResults, true);

	for (FHitResult HitResult : HitResults)
	{
		AARPGCharacter* OtherCharacter = Cast<AARPGCharacter>(HitResult.GetActor());
		if (OtherCharacter && OtherCharacter->GetCombatManager()->GetCharacterCamp() != CharacterCamp)
		{
			return TryExecuteVisceralAttackToTarget(OtherCharacter);
		}
	}
	return false;
}

bool UARPGCombatManager::TryExecuteVisceralAttackToTarget(AARPGCharacter* Target)
{
	if (Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName("State.Break"))))
	{
		if (OwnerCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Common.VisceralAttack")))))
		{
			FGameplayEventData EventData;
			EventData.Instigator = OwnerCharacter.Get();
			EventData.Target = Target;
			FGameplayEffectContext EffectContext;

			OwnerCharacter->GetARPGAbilitySystemComponent()->SendGameplayEventToActor(
				Target, FGameplayTag::RequestGameplayTag(FName("Event.VisceralAttack")), EventData);
			OwnerCharacter->GetARPGAbilitySystemComponent()->SendGameplayEventToActor(
				OwnerCharacter.Get(), FGameplayTag::RequestGameplayTag(FName("Event.Ability.Move")), EventData);

			return true;
		}
	}
	return false;
}

void UARPGCombatManager::ApplyHitReact(EARPGHitReactDirection Direction)
{
}

void UARPGCombatManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
