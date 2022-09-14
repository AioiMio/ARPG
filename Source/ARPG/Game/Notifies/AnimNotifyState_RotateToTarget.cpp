// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_RotateToTarget.h"

#include "RootMotionModifier_SkewWarp.h"
#include "ARPG/Game/Components/ARPGMotionWarpingComponent.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotifyState_RotateToTarget::UAnimNotifyState_RotateToTarget(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	URootMotionModifier_SkewWarp* SkewWarp = CreateDefaultSubobject<URootMotionModifier_SkewWarp>(TEXT("SkewWarp"));
	SkewWarp->WarpTargetName = FName("Target");
	SkewWarp->bWarpTranslation = false;
	SkewWarp->RotationType = EMotionWarpRotationType::Facing;
	RootMotionModifier = SkewWarp;
}

void UAnimNotifyState_RotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                  UAnimSequenceBase* Animation,
                                                  float TotalDuration,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
	if (Character.IsValid() && Character->IsLocallyControlled())
	{
		AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(Character);
		if (PlayerCharacter && PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.2f)
		{
			FVector TargetLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetCharacterMovement()->
				GetCurrentAcceleration();
			PlayerCharacter->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocation(TargetLocation);
			if (!PlayerCharacter->HasAuthority())
			{
				PlayerCharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
					"Target", TargetLocation);
			}
			return;
		}

		if (bAutoFaceTarget)
		{
			if (AARPGCharacter* Target = Character->GetTargetManager()->GetLockOnTarget())
			{
				FVector TargetLocation = Target->GetActorLocation();
				Character->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocation(TargetLocation);
				if (!Character->HasAuthority())
				{
					Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
				}
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
