// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueStatic_HitEffects.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Category = "GameplayCue")
class ARPG_API UGameplayCueStatic_HitEffects : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
