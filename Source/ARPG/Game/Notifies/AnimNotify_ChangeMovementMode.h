// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ChangeMovementMode.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "ChangeMovementMode"))
class ARPG_API UAnimNotify_ChangeMovementMode : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_ChangeMovementMode();

	virtual void Notify(USkeletalMeshComponent* MeshComp,
	                    UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> MovementMode;
};
