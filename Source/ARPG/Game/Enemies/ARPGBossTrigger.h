// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Core/ARPGMechanism.h"
#include "GameFramework/Actor.h"
#include "ARPGBossTrigger.generated.h"

class AARPGCharacter;
class AARPGBossCharacter;
class UBoxComponent;

UCLASS()
class ARPG_API AARPGBossTrigger : public AActor
{
	GENERATED_BODY()

public:
	AARPGBossTrigger();

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	void ResetTrigger();

protected:
	virtual void BeginPlay() override;

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

	UFUNCTION()
	void OnBossDied(AARPGCharacter* Character);

	UPROPERTY(Replicated)
	bool bTriggered;

	UPROPERTY(EditAnywhere)
	AARPGMechanism* FogGate;
};
