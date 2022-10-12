// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackAndTrail.h"

#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Components/AGR_CombatManager.h"

UAnimNotifyState_AttackAndTrail::UAnimNotifyState_AttackAndTrail()
{
	FirstSocketName = FName("End");
	SecondSocketName = FName("Start");
}

void UAnimNotifyState_AttackAndTrail::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                              UAnimSequenceBase* Animation,
                                              float TotalDuration,
                                              const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		const AActor* Owner = MeshComp->GetOwner();
		if (Owner && Owner->HasAuthority())
		{
			UARPGCombatManager* CombatManager = Owner->FindComponentByClass<UARPGCombatManager>();
			if(!IsValid(CombatManager))
			{
				// 	Note: We might also want to check if Owner is not a AAnimationEditorPreviewActor actor (notifiers are triggered in persona preview viewport)
				return;
			}
			CombatManager->SetCurrentAttackType(AttackType);
			CombatManager->SetCurrentHitReactType(HitReactType);
			CombatManager->SetCurrentSkillMultiplier(SkillMultiplier);
			CombatManager->StartTrace();
		}
	}
	
	// if (AARPGCharacter* Character = Cast<AARPGCharacter>(MeshComp->GetOuter()))
	// {
	// 	AARPGWeapon* RightHandWeapon = Character->GetEquipmentManager()->GetCurrentRightHandWeapon();
	// 	if (RightHandWeapon)
	// 	{
	// 		RightHandWeaponMesh = RightHandWeapon->GetMesh();
	// 		Super::NotifyBegin(RightHandWeaponMesh, Animation, TotalDuration, EventReference);
	// 	}
	// }
}

void UAnimNotifyState_AttackAndTrail::NotifyTick(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	// if (RightHandWeaponMesh)
	// {
	// 	Super::NotifyTick(RightHandWeaponMesh, Animation, FrameDeltaTime, EventReference);
	// }
}

void UAnimNotifyState_AttackAndTrail::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		const AActor* Owner = MeshComp->GetOwner();
		if (Owner && Owner->HasAuthority())
		{
			UAGR_CombatManager* CombatManager = Owner->FindComponentByClass<UAGR_CombatManager>();
			if(!IsValid(CombatManager))
			{
				// 	Note: We might also want to check if Owner is not a AAnimationEditorPreviewActor actor (notifiers are triggered in persona preview viewport)
				return;
			}

			CombatManager->EndTrace();
		}
	}
	
	// if (RightHandWeaponMesh)
	// {
	// 	Super::NotifyEnd(RightHandWeaponMesh, Animation, EventReference);
	// }
}
