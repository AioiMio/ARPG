// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "ARPGProjectile.generated.h"

class UArrowComponent;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class ARPG_API AARPGProjectile : public AActor
{
	GENERATED_BODY()

public:
	AARPGProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float Range = 1000.f;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UArrowComponent* Arrow;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

protected:
	virtual void BeginPlay() override;
};
