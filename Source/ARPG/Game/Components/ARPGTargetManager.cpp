// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGTargetManager.h"

#include "EngineUtils.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

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

void UARPGTargetManager::TickComponent(float DeltaTime,
                                       ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter.IsValid() && bIsLockingOn)
	{
		
		
		if (LockOnTarget && LockOnTarget->IsAlive())
		{
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), LockOnTarget->GetActorLocation());
			Rotation.Roll = 0.0f;
			
			if (OwnerCharacter->GetController())
			{
				OwnerCharacter->GetController()->SetControlRotation(
					UKismetMathLibrary::RInterpTo(OwnerCharacter->GetController()->GetControlRotation(), Rotation,
					                              DeltaTime, 3.0f));
			}

			if (bFaceToTarget)
			{
				OwnerCharacter->SetActorRotation(UKismetMathLibrary::RInterpTo(OwnerCharacter->GetActorRotation(), Rotation,DeltaTime, 20.0f));
			}
		}
		else
		{
			LockOnTarget = nullptr;
			bIsLockingOn = false;
			// OwnerPlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			// OwnerPlayerCharacter->bUseControllerRotationYaw = false;
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

void UARPGTargetManager::LockOn_Implementation()
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
		LastLockOnTarget = LockOnTarget;
		SetLockOnTarget(nullptr);
		OnLockOnStateChanged(false);
		return false;
	}

	OtherCharacters.Empty();
	LockOnCharacterIndex = -1;
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AARPGCharacter* Character = Cast<AARPGCharacter>(*It);
		if (Character == nullptr || Character == OwnerCharacter || !Character->IsAlive())
		{
			continue;
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

	if (LockOnCharacterIndex == -1)
	{
		return false;
	}

	SetLockOnTarget(OtherCharacters[LockOnCharacterIndex]);
	OnLockOnStateChanged(true);

	return true;
}

void UARPGTargetManager::ChangeTarget_Implementation()
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
			LastLockOnTarget = LockOnTarget;
			SetLockOnTarget(OtherCharacters[LockOnCharacterIndex]);
			OnLockOnStateChanged(true);
			return;
		}
	}
}

void UARPGTargetManager::OnRep_bIsLockingOn(bool bOldIsLockingOn)
{
	OnLockOnStateChanged(bIsLockingOn);
}

void UARPGTargetManager::OnRep_bFaceToTarget(bool bOldFaceToTarget)
{
}

void UARPGTargetManager::OnRep_LockOnTarget(AARPGCharacter* OldLockOnTarget)
{
	if (OldLockOnTarget)
	{
		if (OwnerCharacter->IsPlayerControlled() && OwnerCharacter->IsLocallyControlled())
		{
			OldLockOnTarget->SetLockOnPointHiddenInGame(true);
		}
	}

	if (LockOnTarget)
	{
		if (OwnerCharacter->IsPlayerControlled() && OwnerCharacter->IsLocallyControlled())
		{
			LockOnTarget->SetLockOnPointHiddenInGame(false);
		}
	}
}

void UARPGTargetManager::OnLockOnStateChanged(bool bNewState)
{
	if (LastLockOnTarget)
	{
		if (OwnerCharacter->IsPlayerControlled() && OwnerCharacter->IsLocallyControlled())
		{
			LastLockOnTarget->SetLockOnPointHiddenInGame(true);
		}
	}

	if (LockOnTarget)
	{
		if (OwnerCharacter->IsPlayerControlled() && OwnerCharacter->IsLocallyControlled())
		{
			LockOnTarget->SetLockOnPointHiddenInGame(false);
		}
	}

	// OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !bNewState;
	// OwnerCharacter->bUseControllerRotationYaw = bNewState;
}

void UARPGTargetManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPGTargetManager, LockOnTarget);
	DOREPLIFETIME(UARPGTargetManager, LastLockOnTarget);
	DOREPLIFETIME(UARPGTargetManager, bIsLockingOn);
}
