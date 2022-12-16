// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueStatic_GunFire.h"

#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

bool UGameplayCueStatic_GunFire::OnExecute_Implementation(AActor* MyTarget,
                                                          const FGameplayCueParameters& Parameters) const
{
	AARPGCharacter* SourceCharacter = Cast<AARPGCharacter>(MyTarget);
	if (SourceCharacter)
	{
		if (AARPGPlayerState* PS = Cast<AARPGPlayerState>(SourceCharacter->GetPlayerState()))
		{
			if (SourceCharacter->IsLocallyControlled() && PS->GetPlayerController() && FireForceFeedbackEffect)
			{
				PS->GetPlayerController()->ClientPlayForceFeedback(FireForceFeedbackEffect);
				PS->GetPlayerController()->ClientStartCameraShake(CameraShake);
			}
		}

		if (AARPGWeapon* Gun = Cast<AARPGWeapon>(SourceCharacter->GetEquipmentManager()->GetCurrentLeftHandWeapon()))
		{
			if (FireEffects.Num() > 0)
			{
				int32 Index = FMath::RandRange(0, FireEffects.Num() - 1);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffects[Index],
														 Gun->GetMesh()->GetSocketTransform(FName("Muzzle")));
			}

			if (FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound,
													  Gun->GetMesh()->GetSocketLocation(FName("Muzzle")), 1.f, 1.f, 0.f,
													  Attenuation);
			}
		}
		return true;
	}

	return false;
}
