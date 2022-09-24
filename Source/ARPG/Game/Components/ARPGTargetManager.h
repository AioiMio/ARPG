// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPGTargetManager.generated.h"

class AARPGCharacter;
class AARPGPlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGTargetManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGTargetManager();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_bIsLockingOn)
	bool bIsLockingOn = false;
	
	float GetActorLockOnPriority(AARPGCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void LockOn();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AARPGCharacter* GetLockOnTarget() { return LockOnTarget; }

	UFUNCTION(BlueprintCallable)
	void SetLockOnTarget(AARPGCharacter* InCharacter);
	
	UFUNCTION(BlueprintCallable)
	bool LockOnPressed();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ChangeTarget();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnOffset = 30.f;
	
	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;
	TWeakObjectPtr<AARPGPlayerCharacter> OwnerPlayerCharacter;

	UPROPERTY(ReplicatedUsing = OnRep_LockOnTarget)
	AARPGCharacter* LockOnTarget;
	UPROPERTY(Replicated)
	AARPGCharacter* LastLockOnTarget;

	UPROPERTY()
	int32 LockOnCharacterIndex;

	UPROPERTY(VisibleAnywhere)
	TArray<AARPGCharacter*> OtherCharacters;

	UFUNCTION()
	void OnRep_bIsLockingOn(bool bOldIsLockingOn);

	UFUNCTION()
	void OnRep_LockOnTarget(AARPGCharacter* OldLockOnTarget);

	void OnLockOnStateChanged(bool bNewState);
};
