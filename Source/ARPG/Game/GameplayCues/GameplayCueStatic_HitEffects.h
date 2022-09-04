// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueStatic_HitEffects.generated.h"

class UForceFeedbackEffect;
class UNiagaraSystem;
class USoundCue;
class USoundAttenuation;

/**
 * 
 */
UCLASS(Blueprintable, Category = "GameplayCue")
class ARPG_API UGameplayCueStatic_HitEffects : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "ForceFeedback")
	UForceFeedbackEffect* HitForceFeedbackEffect;
	
	UPROPERTY(EditAnywhere, Category = "NiagaraSystem")
	TArray<UNiagaraSystem*> NiagaraSystems;

	UPROPERTY(EditAnywhere, Category = "SoundEffects")
	USoundCue* BloodSplashSound;

	UPROPERTY(EditAnywhere, Category = "SoundEffects")
	USoundAttenuation* Attenuation;
};
