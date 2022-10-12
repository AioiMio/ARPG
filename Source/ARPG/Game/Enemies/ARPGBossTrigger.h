// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPGBossTrigger.generated.h"

class AARPGBossCharacter;
class UBoxComponent;

UCLASS()
class ARPG_API AARPGBossTrigger : public AActor
{
	GENERATED_BODY()

public:
	AARPGBossTrigger();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AARPGBossCharacter> BossClass;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                       AActor* OtherActor,
	                       UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex,
	                       bool bFromSweep,
	                       const FHitResult& SweepResult);

	bool bTriggered;
};
