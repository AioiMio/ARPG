// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Types/Types.h"
#include "Components/AGR_CombatManager.h"
#include "ARPGCombatManager.generated.h"

UENUM(BlueprintType)
enum class EAttackHitType : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Heavy UMETA(DisplayName = "Heavy"),
	KnockDown UMETA(DisplayName = "KnockDown"),
	KnockUp UMETA(DisplayName = "KnockUp"),
	KnockBack UMETA(DisplayName = "KnockBack"),
	
	MAX UMETA(DisplayName = "DefaultMAX")
};


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

	FORCEINLINE void SetCurrentAttackHitType(EAttackHitType HitType) { CurrentAttackHitType = HitType; }
	FORCEINLINE EAttackHitType GetCurrentAttackHitType() const { return CurrentAttackHitType; }

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;

	FGameplayTag HitEventTag;

	EAttackHitType CurrentAttackHitType = EAttackHitType::Normal;
	
	UFUNCTION()
	void OnAttackHit(FHitResult Hit, UPrimitiveComponent* Mesh);

	UFUNCTION()
	void ApplyHitReact(EARPGHitReactDirection Direction);
};
