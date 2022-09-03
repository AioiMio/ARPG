// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPGHUDWidget.generated.h"

class USizeBox;
class UProgressBar;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UARPGHUDWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	/**
	* Attribute setters
	*/
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float InMaxHealth);

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthRegenRate(float HealthRegenRate);

	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(float InMaxStamina);

	UFUNCTION(BlueprintCallable)
	void SetCurrentStamina(float CurrentStamina);	

	UFUNCTION(BlueprintCallable)
	void SetStaminaPercentage(float StaminaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStaminaRegenRate(float StaminaRegenRate);

	UFUNCTION(BlueprintCallable)
	void SetMaxMana(float InMaxMana);

	UFUNCTION(BlueprintCallable)
	void SetCurrentMana(float CurrentMana);

	UFUNCTION(BlueprintCallable)
	void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetManaRegenRate(float ManaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExperience(int32 Experience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHeroLevel(int32 HeroLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGold(int32 Gold);

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* HealthBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* ManaBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* StaminaBox;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Health;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBottom;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Mana;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBottom;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Stamina;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBottom;

	UPROPERTY(VisibleAnywhere)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere)
	float MaxMana;
	UPROPERTY(VisibleAnywhere)
	float MaxStamina;

	bool bCanChangeHealthBottom;
	bool bCanChangeManaBottom;
	bool bCanChangeStaminaBottom;
	
	float TargetHealthPercent;
	float TargetManaPercent;
	float TargetStaminaPercent;

	float ChangeBottomDelay = 0.7f;
	
	FTimerHandle TimerHandle_HealthBottomDelay;
	FTimerHandle TimerHandle_ManaBottomDelay;
	FTimerHandle TimerHandle_StaminaBottomDelay;

	UFUNCTION()
	void ChangeHealthBottomElapsed();
	
	UFUNCTION()
	void ChangeManaBottomElapsed();
	
	UFUNCTION()
	void ChangeStaminaBottomElapsed();
};
