// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTCompositeNode_Random.h"

UARPGBTCompositeNode_Random::UARPGBTCompositeNode_Random(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Random";
}

int32 UARPGBTCompositeNode_Random::GetNextChildHandler(FBehaviorTreeSearchData& SearchData,
	int32 PrevChild,
	EBTNodeResult::Type LastResult) const
{
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// newly activated: start from random
		NextChildIdx = FMath::RandRange(0, GetChildrenNum() - 1);
	}

	return NextChildIdx;
}
