// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "ARPGBTCompositeNode_Random.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGBTCompositeNode_Random : public UBTCompositeNode
{
	GENERATED_BODY()

public:
	UARPGBTCompositeNode_Random(const FObjectInitializer& ObjectInitializer);

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
};
