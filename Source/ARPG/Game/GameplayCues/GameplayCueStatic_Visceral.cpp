// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueStatic_Visceral.h"

#include "NiagaraFunctionLibrary.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

bool UGameplayCueStatic_Visceral::OnExecute_Implementation(AActor* MyTarget,
                                                               const FGameplayCueParameters& Parameters) const
{
	AARPGCharacter* InstigatorCharacter = Cast<AARPGCharacter>(Parameters.EffectContext.GetEffectCauser());
	AARPGCharacter* TargetCharacter = Cast<AARPGCharacter>(MyTarget);
	if (InstigatorCharacter)
	{
		if (AARPGPlayerState* PS = Cast<AARPGPlayerState>(InstigatorCharacter->GetPlayerState()))
		{
			if (PS->GetPlayerController() && ForceFeedbackEffect)
			{
				PS->GetPlayerController()->ClientPlayForceFeedback(ForceFeedbackEffect);
				PS->GetPlayerController()->ClientStartCameraShake(CameraShake);
			}
		}
	}

	if (TargetCharacter)
	{
		if (NiagaraSystems.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, NiagaraSystems.Num() - 1);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystems[Index], TargetCharacter->GetMesh()->GetSocketLocation(FName("spine_03")) + FVector(30.f, 0.f, 0.f),
														   TargetCharacter->GetMesh()->GetSocketRotation(FName("spine_03")));
		}

		if (BloodSounds)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BloodSounds, TargetCharacter->GetMesh()->GetSocketLocation(FName("spine_03")), 1.f, 1.f, 0.f, Attenuation);
		}
		
		if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(TargetCharacter->GetController()))
		{
			PC->ClientPlayForceFeedback(ForceFeedbackEffect);
			PC->ClientStartCameraShake(CameraShake);
		}
	}

	return true;
}
