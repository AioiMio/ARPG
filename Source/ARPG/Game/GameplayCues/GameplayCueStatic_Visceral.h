// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueStatic_Visceral.generated.h"

class USoundCue;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class ARPG_API UGameplayCueStatic_Visceral : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ForceFeedback")
	UForceFeedbackEffect* ForceFeedbackEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "NiagaraSystem")
	TArray<UNiagaraSystem*> NiagaraSystems;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* BloodSounds;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundAttenuation* Attenuation;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;
};
