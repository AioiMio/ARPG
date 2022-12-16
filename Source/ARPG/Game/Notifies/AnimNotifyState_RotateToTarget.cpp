// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_RotateToTarget.h"

#include "RootMotionModifier_SkewWarp.h"
#include "ARPG/Game/Components/ARPGMotionWarpingComponent.h"
#include "ARPG/Game/Components/ARPGTargetManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
		if (bAutoFaceTarget)
		{
			if (AARPGCharacter* Target = Character->GetTargetManager()->GetLockOnTarget())
			{
				if (WarpTarget == EWarpTargetType::Component)
				{
					Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromComponent("Target", Target->GetLockOnPointComponent(), "", true);
					if (!Character->HasAuthority())
					{
						Character->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromComponent("Target", Target->GetLockOnPointComponent(), "", true);
					}
					return;
				}
				
				FVector TargetLocation = Target->GetLockOnPointComponent()->GetComponentLocation();
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
					TargetLocation, Character->GetActorLocation());
				LookAtRotation.Pitch = 0.f;
				LookAtRotation.Roll = 0.f;
				TargetLocation.X += LookAtRotation.Vector().X * Offset;
				TargetLocation.Y += LookAtRotation.Vector().Y * Offset;
				TargetLocation.Z = Target->GetActorLocation().Z - Target->GetCapsuleComponent()->
				                                                          GetScaledCapsuleHalfHeight();

				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, Target->GetLockOnPointComponent()->GetComponentLocation());
				TargetRotation.Pitch = 0.f;
				TargetRotation.Roll = 0.f;

				if (Offset != 0.f)
				{
					// Character->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocationAndRotation(TargetLocation, TargetRotation);
					Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("Target", TargetLocation, TargetRotation);
					if (!Character->HasAuthority())
					{
						Character->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocationAndRotation("Target", TargetLocation, TargetRotation);
					}
				}
				else
				{
					Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
					if (!Character->HasAuthority())
					{
						Character->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
					}
				}
				return;
			}
		}

		AARPGPlayerCharacter* PlayerCharacter = Cast<AARPGPlayerCharacter>(Character);
		if (PlayerCharacter && PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.2f)
		{
			FVector TargetLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetCharacterMovement()->
				GetCurrentAcceleration();
			// PlayerCharacter->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocation(TargetLocation);
			PlayerCharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
			if (!PlayerCharacter->HasAuthority())
			{
				PlayerCharacter->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocation(
					"Target", TargetLocation);
			}
		}
	}
}

void UAnimNotifyState_RotateToTarget::NotifyTick(USkeletalMeshComponent* MeshComp,
                                                 UAnimSequenceBase* Animation,
                                                 float FrameDeltaTime,
                                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bShouldTick && WarpTarget == EWarpTargetType::Transform)
	{
		Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
		if (Character.IsValid() && Character->IsLocallyControlled())
		{
			if (bAutoFaceTarget)
			{
				if (AARPGCharacter* Target = Character->GetTargetManager()->GetLockOnTarget())
				{
					FVector TargetLocation = Target->GetLockOnPointComponent()->GetComponentLocation();
					FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
						Character->GetActorLocation(), TargetLocation);
					LookAtRotation.Pitch = 0.f;
					LookAtRotation.Roll = 0.f;
					TargetLocation.X += LookAtRotation.Vector().X * Offset;
					TargetLocation.Y += LookAtRotation.Vector().Y * Offset;
					TargetLocation.Z = Target->GetActorLocation().Z - Target->GetCapsuleComponent()->
					                                                          GetScaledCapsuleHalfHeight();

					FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, Target->GetLockOnPointComponent()->GetComponentLocation());
					TargetRotation.Pitch = 0.f;
					TargetRotation.Roll = 0.f;

					if (Offset != 0.f)
					{
						// Character->GetMotionWarpingComponent()->ServerSetMotionWarpingTargetFromLocationAndRotation(TargetLocation, TargetRotation);
						Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("Target", TargetLocation, TargetRotation);
						if (!Character->HasAuthority())
						{
							Character->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocationAndRotation("Target", TargetLocation, TargetRotation);
						}
					}
					else
					{
						Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
						if (!Character->HasAuthority())
						{
							Character->GetMotionWarpingComponent()->ServerAddOrUpdateWarpTargetFromLocation("Target", TargetLocation);
						}
					}
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
