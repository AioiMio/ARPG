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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLockingOn = false;
	
	float GetActorLockOnPriority(AARPGCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void LockOn();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AARPGCharacter* GetLockOnTarget() { return LockOnTarget.Get(); }

	UFUNCTION(BlueprintCallable)
	void SetLockOnTarget(AARPGCharacter* InCharacter);
	
	UFUNCTION(BlueprintCallable)
	bool LockOnPressed();

	UFUNCTION(BlueprintCallable)
	void ChangeTarget();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnOffset = 30.f;
	
	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;
	TWeakObjectPtr<AARPGPlayerCharacter> OwnerPlayerCharacter;

	UPROPERTY(Replicated)
	TWeakObjectPtr<AARPGCharacter> LockOnTarget;

	UPROPERTY()
	int32 LockOnCharacterIndex;

	UPROPERTY(VisibleAnywhere)
	TArray<AARPGCharacter*> OtherCharacters;
};
