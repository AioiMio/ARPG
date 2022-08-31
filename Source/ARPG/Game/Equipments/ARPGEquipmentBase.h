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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
