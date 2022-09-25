// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueStatic_Break.generated.h"

class USoundCue;
/**
 * 
 */
UCLASS()
class ARPG_API UGameplayCueStatic_Break : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ForceFeedback")
	UForceFeedbackEffect* BreakForceFeedbackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* BreakSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundAttenuation* Attenuation;
};
