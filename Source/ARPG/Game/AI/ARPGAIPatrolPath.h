// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPGAIPatrolPath.generated.h"

UCLASS()
class ARPG_API AARPGAIPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	AARPGAIPatrolPath();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FVector> PatrolPath;

protected:
	virtual void BeginPlay() override;
};
