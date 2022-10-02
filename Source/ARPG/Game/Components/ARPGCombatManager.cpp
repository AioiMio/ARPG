// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCombatManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ARPGAbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ARPG/Game/Abilities/ARPGGameplayEffect_HitReact.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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
			if (AARPGCharacter* HitCharacter = Cast<AARPGCharacter>(Hit.GetActor()))
			{
				FGameplayEventData HitEventData;
				HitEventData.Instigator = GetOwner();
				HitEventData.Target = HitCharacter;
				HitEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitEventTag, HitEventData);

				SendHitEventToActor(HitCharacter, Hit, CurrentAttackType, CurrentHitReactType, CurrentSkillMultiplier);
			}
		}
	}
}

void UARPGCombatManager::SendHitEventToActor(AActor* Target, FHitResult Hit, EAttackType AttackType, EHitReactType HitReactType, float Multiplier)
{
	if (AARPGCharacter* Character = Cast<AARPGCharacter>(Target))
	{
		UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent();
		UAbilitySystemComponent* SourceASC = OwnerCharacter->GetAbilitySystemComponent();
		if (TargetASC && SourceASC)
		{
			if (AttackType == EAttackType::Charge && ChargeDamageEffectClass)
			{
				FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
				DamageEffectContext.AddInstigator(GetOwner(), GetOwner());
				DamageEffectContext.AddHitResult(Hit);
				FGameplayEffectSpecHandle DamageEffectSpecHandle = TargetASC->MakeOutgoingSpec(
					ChargeDamageEffectClass, 1, DamageEffectContext);
				DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"),
																	 Multiplier);
				SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetASC);
			}
			else if (DamageEffectClass)
			{
				FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
				DamageEffectContext.AddInstigator(GetOwner(), GetOwner());
				DamageEffectContext.AddHitResult(Hit);
				FGameplayEffectSpecHandle DamageEffectSpecHandle = TargetASC->MakeOutgoingSpec(
					DamageEffectClass, 1, DamageEffectContext);
				DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"),
				                                                     Multiplier);
				SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetASC);
			}

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

void UARPGCombatManager::ServerSendGameplayEventToActor_Implementation(AActor* Actor,
	FGameplayTag EventTag,
	FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
}

void UARPGCombatManager::ClientSendGameplayEventToActor_Implementation(AActor* Actor,
	FGameplayTag EventTag,
	FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
}

bool UARPGCombatManager::TryVisceralAttack()
{
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
		if (AARPGCharacter* OtherCharacter = Cast<AARPGCharacter>(HitResult.GetActor()))
		{
			if (OtherCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(
				FGameplayTag::RequestGameplayTag(FName("State.Break"))))
			{
				if (OwnerCharacter->GetActorForwardVector().Dot(OtherCharacter->GetActorForwardVector()) < 0.f)
				{
					if (OwnerCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
						FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Common.VisceralAttack")))))
					{
						FGameplayEventData EventData;
						EventData.Instigator = OwnerCharacter.Get();
						EventData.Target = OtherCharacter;
						FGameplayEffectContext EffectContext;
						OtherCharacter->GetARPGAbilitySystemComponent()->SendGameplayEventToOwner(FGameplayTag::RequestGameplayTag(FName("Event.VisceralAttack")), EventData);
						OwnerCharacter->GetARPGAbilitySystemComponent()->SendGameplayEventToOwner(FGameplayTag::RequestGameplayTag(FName("Event.Ability.Move")), EventData);
						// ServerSendGameplayEventToActor(OtherCharacter, FGameplayTag::RequestGameplayTag(FName("Event.VisceralAttack")), EventData);
						// ServerSendGameplayEventToActor(OwnerCharacter.Get(), FGameplayTag::RequestGameplayTag(FName("Event.Ability.Move")), EventData);
					
						return true;
					}
					return false;
				}
			}
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
