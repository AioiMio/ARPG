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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetHealthPercentage(float InPercent);
	void SetCharacterName(const FText& NewName);
	void ShowDamageNumber(float ActualDelta);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOwnerActor(AActor* Owner) { OwnerActor = Owner; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetOwnerActor() { return OwnerActor; }

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthTop;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBottom;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageNum;

	float ChangeHealthBottomDelay = 1.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 Damage;

	bool bCanHealthChangeTimerSet;
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
