// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_MoveAndRotate.generated.h"

class UCurveVector;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveAndRotateDelegate);

/**
 * 
 */
UCLASS()
class ARPG_API UAbilityTask_MoveAndRotate : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_MoveAndRotate(const FObjectInitializer& ObjectInitializer);
	
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityIsEnding) override;
	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

	/** Move to the specified location, using the vector curve (range 0 - 1) if specified, otherwise the float curve (range 0 - 1) or fallback to linear interpolation */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_MoveAndRotate* MoveAndRotate(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, FRotator Rotation, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve);

	UPROPERTY(BlueprintAssignable)
	FMoveAndRotateDelegate		OnTargetReached;

protected:
	bool bIsFinished;
	
	UPROPERTY(Replicated)
	FRotator StartRotation;

	UPROPERTY(Replicated)
	FRotator TargetRotation;

	UPROPERTY(Replicated)
	FVector StartLocation;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	float DurationOfMovement;

	float TimeMoveStarted;

	float TimeMoveWillEnd;

	UPROPERTY(Replicated)
	UCurveFloat* LerpCurve;

	UPROPERTY(Replicated)
	UCurveVector* LerpCurveVector;
};
