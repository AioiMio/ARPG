// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGMotionWarpingComponent.h"

#include "ARPG/Game/Core/ARPGCharacter.h"

UARPGMotionWarpingComponent::UARPGMotionWarpingComponent(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	OnwerCharacter = Cast<AARPGCharacter>(GetCharacterOwner());
}

void UARPGMotionWarpingComponent::ServerAddOrUpdateWarpTargetFromLocation_Implementation(FName InTargetName, FVector TargetLocation)
{
	AddOrUpdateWarpTargetFromLocation(InTargetName, TargetLocation);
}

void UARPGMotionWarpingComponent::ServerAddOrUpdateWarpTargetFromLocationAndRotation_Implementation(FName InTargetName,
	FVector TargetLocation,
	FRotator TargetRotation)
{
	AddOrUpdateWarpTargetFromLocationAndRotation(InTargetName, TargetLocation, TargetRotation);
}

void UARPGMotionWarpingComponent::ServerAddOrUpdateWarpTargetFromComponent_Implementation(FName WarpTargetName,
	const USceneComponent* Component,
	FName BoneName,
	bool bFollowComponent)
{
	AddOrUpdateWarpTargetFromComponent(WarpTargetName, Component, BoneName, bFollowComponent);
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

void UARPGMotionWarpingComponent::SetMotionWarpingTargetFromLocationAndRotation(FVector TargetLocation,
	FRotator TargetRotation)
{
	if (OnwerCharacter.IsValid())
	{
		if (!OnwerCharacter->HasAuthority())
		{
			AddOrUpdateWarpTargetFromLocationAndRotation(TargetName, TargetLocation, TargetRotation);
		}
		ServerSetMotionWarpingTargetFromLocationAndRotation(TargetLocation, TargetRotation);
	}
}

void UARPGMotionWarpingComponent::ServerSetMotionWarpingTargetFromLocationAndRotation_Implementation(
	FVector TargetLocation,
	FRotator TargetRotation)
{
	MulticastSetMotionWarpingTargetFromLocationAndRotation(TargetLocation, TargetRotation);
}

void UARPGMotionWarpingComponent::MulticastSetMotionWarpingTargetFromLocationAndRotation_Implementation(
	FVector TargetLocation,
	FRotator TargetRotation)
{
	AddOrUpdateWarpTargetFromLocationAndRotation(TargetName, TargetLocation, TargetRotation);
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
