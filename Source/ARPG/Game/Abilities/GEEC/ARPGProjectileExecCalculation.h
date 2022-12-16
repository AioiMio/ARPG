// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ARPGProjectileExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGProjectileExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UARPGProjectileExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	FGameplayTag ChanceTag;
};
