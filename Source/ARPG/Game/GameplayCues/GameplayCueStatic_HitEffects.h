// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueStatic_HitEffects.generated.h"

class UForceFeedbackEffect;
class UNiagaraSystem;
class USoundCue;
class USoundAttenuation;
class UCameraShakeBase;
class AARPGCharacter;

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
	UPROPERTY(EditDefaultsOnly, Category = "ForceFeedback")
	UForceFeedbackEffect* HitForceFeedbackEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "NiagaraSystem")
	TArray<UNiagaraSystem*> NiagaraSystems;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* BloodSplashSound;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundAttenuation* Attenuation;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitAnimRateScale = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitFeedbackEffectDuration = 0.08f;
};
