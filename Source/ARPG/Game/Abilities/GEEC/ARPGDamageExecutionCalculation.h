// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ARPGDamageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UARPGDamageExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	FGameplayTag ChanceTag;

	float ChancePostureDamageMultiplier = 5.f;
};
