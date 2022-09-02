// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AGR_CombatManager.h"
#include "ARPGCombatManager.generated.h"

class AARPGCharacter;
class UForceFeedbackEffect;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGCombatManager : public UAGR_CombatManager
{
	GENERATED_BODY()

public:
	UARPGCombatManager();

	FORCEINLINE TArray<AActor*> GetIgnoredActors() { return IgnoredActors; }

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;

	FGameplayTag HitEventTag;

	UPROPERTY(EditAnywhere, Category = "ForceFeedback")
	UForceFeedbackEffect* HitFeedback;
	
	UFUNCTION()
	void OnAttackHit(FHitResult Hit, UPrimitiveComponent* Mesh);
};
