// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTDecorator_Probability.h"

bool UARPGBTDecorator_Probability::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	return FMath::RandRange(0.f, 1.f) <= FMath::Clamp(Probability, 0.f, 1.f);
}
