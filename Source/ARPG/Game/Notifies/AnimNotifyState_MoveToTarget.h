// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "AnimNotifyState_MoveToTarget.generated.h"

class AARPGCharacter;

/**
 * 
 */
UCLASS(meta = (DisplayName = "MotionWarping_MoveToTarget"))
class ARPG_API UAnimNotifyState_MoveToTarget : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UAnimNotifyState_MoveToTarget(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
							 UAnimSequenceBase* Animation,
							 float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
						   UAnimSequenceBase* Animation,
						   const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<AARPGCharacter> Character;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool bAutoFaceTarget = false;
};
