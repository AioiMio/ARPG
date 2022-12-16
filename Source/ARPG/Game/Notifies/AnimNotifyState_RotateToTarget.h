// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "ARPG/Game/Types/Types.h"
#include "AnimNotifyState_RotateToTarget.generated.h"

class AARPGCharacter;

/**
 * 
 */
UCLASS(meta = (DisplayName = "MotionWarping_RotateToTarget"))
class ARPG_API UAnimNotifyState_RotateToTarget : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UAnimNotifyState_RotateToTarget(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
	                         UAnimSequenceBase* Animation,
	                         float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
	                        UAnimSequenceBase* Animation,
	                        float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
	                       UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<AARPGCharacter> Character;

	UPROPERTY(EditAnywhere, Category = "Warp")
	bool bAutoFaceTarget = false;

	UPROPERTY(EditAnywhere, Category = "Warp")
	bool bShouldTick = false;

	UPROPERTY(EditAnywhere, Category = "Warp")
	EWarpTargetType WarpTarget = EWarpTargetType::Transform;

	UPROPERTY(EditAnywhere, Category = "Warp", meta = (EditCondition = "WarpTarget == EWarpTargetType::Transform", EditConditionHides))
	float Offset;
};
