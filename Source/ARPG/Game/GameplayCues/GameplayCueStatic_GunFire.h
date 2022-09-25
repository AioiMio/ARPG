// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueStatic_GunFire.generated.h"

class UForceFeedbackEffect;
class UParticleSystem;
class USoundCue;
class USoundAttenuation;
class UCameraShakeBase;
class AARPGCharacter;

/**
 * 
 */
UCLASS()
class ARPG_API UGameplayCueStatic_GunFire : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ForceFeedback")
	UForceFeedbackEffect* FireForceFeedbackEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<UParticleSystem*> FireEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundAttenuation* Attenuation;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;
};
