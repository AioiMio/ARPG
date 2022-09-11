// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "ARPG/Game/Components/ARPGCombatManager.h"
#include "AnimNotifyState_AttackAndTrail.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "AttackAndTrail"))
class ARPG_API UAnimNotifyState_AttackAndTrail : public UAnimNotifyState_Trail
{
	GENERATED_BODY()

public:
	UAnimNotifyState_AttackAndTrail();
	
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
	UPROPERTY()
	USkeletalMeshComponent* RightHandWeaponMesh;

	UPROPERTY()
	USkeletalMeshComponent* LeftHandWeaponMesh;

	UPROPERTY(EditAnywhere)
	EAttackHitType AttackHitType = EAttackHitType::Normal;
};
