// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SendEventToOwner.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "SendEventToOwner"))
class ARPG_API UAnimNotify_SendEventToOwner : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp,
	                    UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayEventData EventPayload;
};
