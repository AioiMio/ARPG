// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "ARPGMotionWarpingComponent.generated.h"

class AARPGCharacter;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGMotionWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()
	
public:
	UARPGMotionWarpingComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Server, Reliable)
	void ServerAddOrUpdateWarpTargetFromLocation(FName InTargetName, FVector TargetLocation);
	UFUNCTION(Server, Reliable)
	void ServerAddOrUpdateWarpTargetFromLocationAndRotation(FName InTargetName, FVector TargetLocation, FRotator TargetRotation);
	UFUNCTION(Server, Reliable)
	void ServerAddOrUpdateWarpTargetFromComponent(FName WarpTargetName, const USceneComponent* Component, FName BoneName, bool bFollowComponent);
	
	void SetMotionWarpingTargetFromLocation(FVector TargetLocation);
	UFUNCTION(Server, Reliable)
	void ServerSetMotionWarpingTargetFromLocation(FVector TargetLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMotionWarpingTargetFromLocation(FVector TargetLocation);

	void SetMotionWarpingTargetFromLocationAndRotation(FVector TargetLocation, FRotator TargetRotation);
	UFUNCTION(Server, Reliable)
	void ServerSetMotionWarpingTargetFromLocationAndRotation(FVector TargetLocation, FRotator TargetRotation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMotionWarpingTargetFromLocationAndRotation(FVector TargetLocation, FRotator TargetRotation);
	
	void RemoveMotionWarpingTarget(FName InTargetName);

	UFUNCTION(Server, Reliable)
	void ServerRemoveMotionWarpingTarget(FName InTargetName);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRemoveMotionWarpingTarget(FName InTargetName);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TargetName = "Target";

	TWeakObjectPtr<AARPGCharacter> OnwerCharacter;
};
