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

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float Range = 1500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float Speed = 5000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	AActor* Target;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TArray<UParticleSystem*> ImpactEffects;

protected:
	virtual void BeginPlay() override;
};
