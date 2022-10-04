// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ARPGBTDecorator_Probability.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGBTDecorator_Probability : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, Category = "Probability")
	float Probability;
};
