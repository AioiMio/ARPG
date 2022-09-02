// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_RotateToTarget.h"

#include "ARPG/Game/Components/ARPGMotionWarpingComponent.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"

void UAnimNotifyState_RotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                  UAnimSequenceBase* Animation,
                                                  float TotalDuration,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
	if (Character.IsValid() && Character->IsLocallyControlled())
	{
		// AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(Character);
		// if (PlayerCharacter && PlayerCharacter->IsMovementInputting())
		// {
		// 	FVector TargetLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetMovementWorldVector() * 1000.f;
		// 	PlayerCharacter->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocation(TargetLocation);
		//
		// 	return;
		// }
		
		if (bAutoFaceTarget)
		{
			if (AARPGCharacter* Target = Character->GetTargetManager()->GetLockOnTarget())
			{
				FVector TargetLocation = Target->GetActorLocation();
				Character->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocation(TargetLocation);
			}
		}
	}
}

void UAnimNotifyState_RotateToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Character = Cast<AARPGCharacter>(MeshComp->GetOuter());

	if (Character.IsValid())
	{
		Character->GetMotionWarpingComponent()->RemoveWarpTarget("Target");
	}
}
