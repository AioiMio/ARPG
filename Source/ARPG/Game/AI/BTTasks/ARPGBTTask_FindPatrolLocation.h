// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPGBTTask_FindPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGBTTask_FindPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector PatrolLocation;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector PatrolIndex;
};
