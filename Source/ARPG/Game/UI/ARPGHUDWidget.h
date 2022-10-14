// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPGHUDWidget.generated.h"

class UTextBlock;
class UImage;
class USizeBox;
class UProgressBar;
class UARPGHealthBarWidget;

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

	UPROPERTY(meta = (BindWidget))
	UImage* RightHandWeaponIcon;
	UPROPERTY(meta = (BindWidget))
	UImage* LeftHandWeaponIcon;

	UPROPERTY(meta = (BindWidget))
	USizeBox* HealthPivotBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* ManaPivotBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* StaminaPivotBox;

	UPROPERTY(meta = (BindWidget))
	UARPGHealthBarWidget* BossHealthBar;
	
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

	UFUNCTION(BlueprintCallable)
	void SetMaxPosture(float InMaxPosture);

	UFUNCTION(BlueprintCallable)
	void SetCurrentPosture(float CurrentPosture);

	UFUNCTION(BlueprintCallable)
	void SetPosturePercentage(float PosturePercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPostureRegenRate(float PostureRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExperience(int32 Experience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHeroLevel(int32 HeroLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGold(int32 Gold);

	UFUNCTION(BlueprintCallable)
	void SetInteractText(const FText& InText);
	
	UFUNCTION(BlueprintCallable)
	void SetMessageText(const FText& InText, float Duration = 3.f);

	UFUNCTION(BlueprintCallable)
	void ShowMainMessage(const FText& Message, const FColor& Color, float Duration);

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* HealthBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* ManaBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* StaminaBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* PostureBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* PostureBar;
	UPROPERTY(meta = (BindWidget))
	USizeBox* InteractBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* MessageBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainMessage;

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
	UPROPERTY(VisibleAnywhere)
	float MaxPosture;

	bool bCanHealthChangedTimerSet;
	bool bCanManaChangedTimerSet;
	bool bCanStaminaChangedTimerSet;

	FTimerDelegate ChangeHealthDelegate;
	FTimerDelegate ChangeStaminaDelegate;
	FTimerDelegate ChangeManaDelegate;
	
	bool bCanChangeHealthBottom;
	bool bCanChangeManaBottom;
	bool bCanChangeStaminaBottom;
	
	float TargetHealthPercent;
	float TargetManaPercent;
	float TargetStaminaPercent;

	float ChangeBottomDelay = 0.5f;
	
	FTimerHandle TimerHandle_HealthBottomDelay;
	FTimerHandle TimerHandle_ManaBottomDelay;
	FTimerHandle TimerHandle_StaminaBottomDelay;
	FTimerHandle TimerHandle_MessageClearDelay;
	FTimerHandle TimerHandle_MainMessageClearDelay;

	// Interact
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	FText InteractText;

	// Message
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Message")
	FText MessageText;

	UFUNCTION()
	void ChangeHealthBottomElapsed();
	
	UFUNCTION()
	void ChangeManaBottomElapsed();
	
	UFUNCTION()
	void ChangeStaminaBottomElapsed();

	UFUNCTION()
	void ClearMessageElapsed();

	UFUNCTION()
	void ClearMainMessageElapsed();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowPostureBar();
	UFUNCTION(BlueprintImplementableEvent)
	void HidePostureBar();

	UFUNCTION(BlueprintNativeEvent)
	void ShowMainMessageAnimation();
	UFUNCTION(BlueprintNativeEvent)
	void HideMainMessageAnimation();
};
