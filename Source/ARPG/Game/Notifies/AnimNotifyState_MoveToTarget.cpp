// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_MoveToTarget.h"

#include "RootMotionModifier_SkewWarp.h"
#include "ARPG/Game/Components/ARPGMotionWarpingComponent.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotifyState_MoveToTarget::UAnimNotifyState_MoveToTarget(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	URootMotionModifier_SkewWarp* SkewWarp = CreateDefaultSubobject<URootMotionModifier_SkewWarp>(TEXT("SkewWarp"));
	SkewWarp->WarpTargetName = FName("Target");
	SkewWarp->bWarpTranslation = true;
	SkewWarp->bIgnoreZAxis = false;
	SkewWarp->RotationType = EMotionWarpRotationType::Facing;
	RootMotionModifier = SkewWarp;
}

void UAnimNotifyState_MoveToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                UAnimSequenceBase* Animation,
                                                float TotalDuration,
                                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
	if (Character.IsValid() && Character->IsLocallyControlled())
	{
		if (bAutoFaceTarget)
		{
			if (AARPGCharacter* Target = Character->GetTargetManager()->GetLockOnTarget())
			{
				FVector TargetLocation = Target->GetActorLocation() - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
				Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
				if (!Character->HasAuthority())
				{
					Character->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
				}
				return;
			}
		}
		
		if (AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(Character))
		{
			FVector ForwardOffset;
			if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.2f)
			{
				ForwardOffset = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration();
			}
			else
			{
				ForwardOffset = PlayerCharacter->GetActorForwardVector();
			}
			ForwardOffset.Normalize();
			FVector TargetLocation = PlayerCharacter->GetActorLocation() + ForwardOffset * 500.f - FVector(0.f, 0.f, PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

			PlayerCharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
			if (!PlayerCharacter->HasAuthority())
			{
				PlayerCharacter->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
			}
		}
	}
}

void UAnimNotifyState_MoveToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp,
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
