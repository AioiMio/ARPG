// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueStatic_HitEffects.h"

#include "NiagaraFunctionLibrary.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

bool UGameplayCueStatic_HitEffects::OnExecute_Implementation(AActor* MyTarget,
                                                             const FGameplayCueParameters& Parameters) const
{
	if (AARPGPlayerState* PS = Cast<AARPGPlayerState>(Parameters.GetInstigator()))
	{
		if (PS->GetPlayerController() && HitForceFeedbackEffect)
		{
			PS->GetPlayerController()->ClientPlayForceFeedback(HitForceFeedbackEffect);
		}
	}
	
	FHitResult HitResult = *Parameters.EffectContext.GetHitResult();
	
	if (NiagaraSystems.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, NiagaraSystems.Num() - 1);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystems[Index], HitResult.Location, HitResult.Normal.Rotation());
	}

	if (BloodSplashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BloodSplashSound, HitResult.Location, HitResult.Normal.Rotation(), 1.f, 1.f, 0.f, Attenuation);
	}

	AARPGPlayerCharacter* VictimCharacter = Cast<AARPGPlayerCharacter>(HitResult.GetActor());
	if (VictimCharacter)
	{
		if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(VictimCharacter->GetController()))
		{
			PC->ClientPlayForceFeedback(HitForceFeedbackEffect);
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
