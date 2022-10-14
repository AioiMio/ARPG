// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ARPGAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGAbilitySystemLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameplayEffect")
	static void SetInstigator(FGameplayEffectContextHandle EffectContext, AActor* InInstigator, AActor* InEffectCauser);

	UFUNCTION(BlueprintCallable, Category = "GameplayEffect")
	static void SetOrigin(FGameplayEffectContextHandle EffectContext, FVector InOrigin);
};
