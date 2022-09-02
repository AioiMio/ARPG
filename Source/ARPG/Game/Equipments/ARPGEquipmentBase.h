// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPGEquipmentBase.generated.h"

class UBoxComponent;
class UArrowComponent;
class USkeletalMeshComponent;

UCLASS()
class ARPG_API AARPGEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AARPGEquipmentBase();

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return SkeletalMeshComponent; }

	UFUNCTION(BlueprintImplementableEvent)
	void JoinWorld();

	UFUNCTION(BlueprintImplementableEvent)
	void LeaveWorld();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLeaveWorld();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
