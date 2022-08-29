// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPGHealthBarWidget.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	void SetHealthPercent(float InPercent);
	void ShowDamageNumber(float ActualDelta);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOwnerActor(AActor* Owner) { OwnerActor = Owner; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetOwnerActor() { return OwnerActor; }

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Health;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBottom;
	UPROPERTY(meta = (BindWidget), Replicated)
	UTextBlock* DamageNum;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
	int32 Damage;
	
	bool bCanChangeHealthBottom;

	FTimerHandle TimerHandle_HealthBottomDelay;
	FTimerHandle TimerHandle_HideDamageNumberDelay;

	UFUNCTION()
	void ChangeHealthBottomElapsed();

	UFUNCTION()
	void HideDamageNumber();

	UPROPERTY()
	AActor* OwnerActor;
};
