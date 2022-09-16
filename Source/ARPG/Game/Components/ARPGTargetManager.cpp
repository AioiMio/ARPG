// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGTargetManager.h"

#include "EngineUtils.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UARPGTargetManager::UARPGTargetManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARPGTargetManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPGCharacter>(GetOuter());
	OwnerPlayerCharacter = Cast<AARPGPlayerCharacter>(GetOuter());
}

void UARPGTargetManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerPlayerCharacter.IsValid() && bIsLockingOn)
	{
		if (LockOnTarget.IsValid() && LockOnTarget->IsAlive())
		{
			FVector Direction = LockOnTarget->GetActorLocation() - OwnerPlayerCharacter->GetActorLocation();
			FRotator Rotation = Direction.Rotation();
			Rotation.Roll = 0.0f;
			// Rotation.Yaw += OwnerPlayerCharacter->GetCameraInputVector().X * LockOnOffset;
			// Rotation.Pitch -= OwnerPlayerCharacter->GetCameraInputVector().Y * LockOnOffset * 0.5f;

			if (OwnerPlayerCharacter->GetController())
			{
				OwnerPlayerCharacter->GetController()->SetControlRotation(
					UKismetMathLibrary::RInterpTo(OwnerPlayerCharacter->GetController()->GetControlRotation(), Rotation,
												  DeltaTime, 3.0f));
			}
		}
		else
		{
			LockOnTarget = nullptr;
			bIsLockingOn = false;
			// PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			// PlayerCharacter->bUseControllerRotationYaw = false;
		}
	}
}

float UARPGTargetManager::GetActorLockOnPriority(AARPGCharacter* TargetCharacter)
{
	if (OwnerPlayerCharacter.IsValid())
	{
		FVector CameraLocation = OwnerPlayerCharacter->GetActorLocation();
		FVector TargetLocation = TargetCharacter->GetActorLocation();

		return (TargetLocation - CameraLocation).Dot(OwnerPlayerCharacter->GetControlRotation().Vector())
			/ FMath::Pow((TargetLocation - CameraLocation).Size(), 1.5f);
	}
	return -1;
}

void UARPGTargetManager::LockOn()
{
	bIsLockingOn = LockOnPressed();
}

void UARPGTargetManager::SetLockOnTarget(AARPGCharacter* InCharacter)
{
	LockOnTarget = InCharacter;
}

bool UARPGTargetManager::LockOnPressed()
{
	if (LockOnTarget != nullptr)
	{
		LockOnTarget->SetLockOnPointHiddenInGame(true);
		// PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		// PlayerCharacter->bUseControllerRotationYaw = false;
		SetLockOnTarget(nullptr);
		return false;
	}

	OtherCharacters.Empty();
	LockOnCharacterIndex = -1;
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AARPGCharacter* Character = Cast<AARPGCharacter>(*It);
		if (Character == nullptr || Character == OwnerCharacter || !Character->IsAlive())
		{
			continue;;
		}

		OtherCharacters.AddUnique(Character);
	}

	if (OtherCharacters.Num() == 0)
	{
		return false;
	}
	OtherCharacters.Sort([&](AARPGCharacter& A, AARPGCharacter& B)
	{
		return GetActorLockOnPriority(&A) > GetActorLockOnPriority(&B);
	});

	for (int32 i = 0; i < OtherCharacters.Num(); i++)
	{
		if (!OwnerCharacter->GetController()->LineOfSightTo(OtherCharacters[i]))
		{
			continue;
		}
		LockOnCharacterIndex = i;
		break;
	}

	// if (LockOnCharacterIndex > OtherCharacters.Num())
	// {
	// 	LockOnCharacterIndex = OtherCharacters.Num();
	// }

	if (LockOnCharacterIndex == -1)
	{
		return false;
	}

	SetLockOnTarget(OtherCharacters[LockOnCharacterIndex]);
	LockOnTarget->SetLockOnPointHiddenInGame(false);
	// PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	// PlayerCharacter->bUseControllerRotationYaw = true;

	return true;
}

void UARPGTargetManager::ChangeTarget()
{
	if (bIsLockingOn)
	{
		for (int32 i = 1; i < OtherCharacters.Num(); i++)
		{
			int32 Index = (LockOnCharacterIndex + i) % OtherCharacters.Num();
			if (!OwnerCharacter->GetController()->LineOfSightTo(OtherCharacters[Index]))
			{
				continue;
			}
			LockOnCharacterIndex = Index;
			LockOnTarget->SetLockOnPointHiddenInGame(true);
			LockOnTarget = OtherCharacters[LockOnCharacterIndex];
			LockOnTarget->SetLockOnPointHiddenInGame(false);
			return;
		}
	}
}

