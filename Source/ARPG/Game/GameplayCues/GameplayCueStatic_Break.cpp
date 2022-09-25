// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueStatic_Break.h"

#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


bool UGameplayCueStatic_Break::OnExecute_Implementation(AActor* MyTarget,
                                                        const FGameplayCueParameters& Parameters) const
{
	AARPGCharacter* SourceCharacter = Cast<AARPGCharacter>(MyTarget);
	if (SourceCharacter)
	{
		if (AARPGPlayerState* PS = Cast<AARPGPlayerState>(SourceCharacter->GetPlayerState()))
		{
			if (SourceCharacter->IsLocallyControlled() && PS->GetPlayerController() && BreakForceFeedbackEffect)
			{
				PS->GetPlayerController()->ClientPlayForceFeedback(BreakForceFeedbackEffect);
			}
		}

		if (BreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, MyTarget->GetActorLocation(), 1.f, 1.f, 0.f,
			                                      Attenuation);
		}
		return true;
	}

	return false;
}
