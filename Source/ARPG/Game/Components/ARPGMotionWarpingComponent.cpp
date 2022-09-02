// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGMotionWarpingComponent.h"

#include "ARPG/Game/Core/ARPGCharacter.h"

UARPGMotionWarpingComponent::UARPGMotionWarpingComponent(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	OnwerCharacter = Cast<AARPGCharacter>(GetCharacterOwner());
}

void UARPGMotionWarpingComponent::SetMotionWarpingTargetFromLocation(FVector TargetLocation)
{
	if (OnwerCharacter.IsValid())
	{
		if (!OnwerCharacter->HasAuthority())
		{
			AddOrUpdateWarpTargetFromLocation(TargetName, TargetLocation);
		}
		ServerSetMotionWarpingTargetFromLocation(TargetLocation);
	}
}

void UARPGMotionWarpingComponent::ServerSetMotionWarpingTargetFromLocation_Implementation(FVector TargetLocation)
{
	MulticastSetMotionWarpingTargetFromLocation(TargetLocation);
}

void UARPGMotionWarpingComponent::MulticastSetMotionWarpingTargetFromLocation_Implementation(FVector TargetLocation)
{
	AddOrUpdateWarpTargetFromLocation(TargetName, TargetLocation);
}

void UARPGMotionWarpingComponent::RemoveMotionWarpingTarget(FName InTargetName)
{
	if (OnwerCharacter.IsValid())
	{
		if (!OnwerCharacter->HasAuthority())
		{
			RemoveWarpTarget(InTargetName);
		}
		ServerRemoveMotionWarpingTarget_Implementation(InTargetName);
	}
}

void UARPGMotionWarpingComponent::ServerRemoveMotionWarpingTarget_Implementation(FName InTargetName)
{
	MulticastRemoveMotionWarpingTarget(InTargetName);
}

void UARPGMotionWarpingComponent::MulticastRemoveMotionWarpingTarget_Implementation(FName InTargetName)
{
	RemoveWarpTarget(InTargetName);
}
