// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGHUDWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UARPGHUDWidget::UARPGHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCanHealthChangedTimerSet = true;
	bCanManaChangedTimerSet = true;
	bCanStaminaChangedTimerSet = true;
	bCanChangeHealthBottom = false;
	bCanChangeManaBottom = false;
	bCanChangeStaminaBottom = false;

	MaxHealth = 0.f;
	MaxMana = 0.f;
	MaxStamina = 0.f;
	
	ChangeHealthDelegate.BindUFunction(this, "ChangeHealthBottomElapsed");
	ChangeStaminaDelegate.BindUFunction(this, "ChangeStaminaBottomElapsed");
	ChangeManaDelegate.BindUFunction(this, "ChangeManaBottomElapsed");
}

void UARPGHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bCanChangeHealthBottom)
	{
		if (Health->Percent >= HealthBottom->Percent)
		{
			bCanChangeHealthBottom = false;
			bCanHealthChangedTimerSet = true;
			HealthBottom->SetPercent(Health->Percent);
		}
		else
		{
			float CurrentPercent = FMath::FInterpConstantTo(HealthBottom->Percent, Health->Percent, InDeltaTime, 0.3f);
			HealthBottom->SetPercent(CurrentPercent);
		}
	}

	if (bCanChangeManaBottom)
	{
		if (Mana->Percent >= ManaBottom->Percent)
		{
			bCanChangeManaBottom = false;
			bCanManaChangedTimerSet = true;
			ManaBottom->SetPercent(Mana->Percent);
		}
		else
		{
			float CurrentPercent = FMath::FInterpConstantTo(ManaBottom->Percent, Mana->Percent, InDeltaTime, 0.3f);
			ManaBottom->SetPercent(CurrentPercent);
		}
	}

	if (bCanChangeStaminaBottom)
	{
		if (Stamina->Percent >= StaminaBottom->Percent)
		{
			bCanChangeStaminaBottom = false;
			bCanStaminaChangedTimerSet = true;
			StaminaBottom->SetPercent(Stamina->Percent);
		}
		else
		{
			float CurrentPercent =
				FMath::FInterpConstantTo(StaminaBottom->Percent, Stamina->Percent, InDeltaTime, 0.3f);
			StaminaBottom->SetPercent(CurrentPercent);
		}
	}
}


/**
* Attribute setters
*/
void UARPGHUDWidget::SetMaxHealth(float InMaxHealth)
{
	HealthBox->SetWidthOverride(InMaxHealth);
	MaxHealth = InMaxHealth;
}

void UARPGHUDWidget::SetCurrentHealth(float CurrentHealth)
{
	SetHealthPercentage(CurrentHealth / MaxHealth);

	HealthPivotBox->SetWidthOverride(CurrentHealth);
}

void UARPGHUDWidget::SetHealthPercentage(float HealthPercentage)
{
	if (Health)
	{
		Health->SetPercent(HealthPercentage);

		if (HealthBottom)
		{
			if (Health->Percent > HealthBottom->Percent)
			{
				bCanChangeHealthBottom = true;
			}
			if (Health->Percent < HealthBottom->Percent && bCanHealthChangedTimerSet)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealthBottomDelay, ChangeHealthDelegate, ChangeBottomDelay, false);
				bCanHealthChangedTimerSet = false;
			}
		}
	}
}

void UARPGHUDWidget::SetMaxStamina(float InMaxStamina)
{
	StaminaBox->SetWidthOverride(InMaxStamina);
	MaxStamina = InMaxStamina;
}

void UARPGHUDWidget::SetCurrentStamina(float CurrentStamina)
{
	SetStaminaPercentage(CurrentStamina / MaxStamina);

	StaminaPivotBox->SetWidthOverride(CurrentStamina);
}

void UARPGHUDWidget::SetStaminaPercentage(float StaminaPercentage)
{
	if (Stamina)
	{
		Stamina->SetPercent(StaminaPercentage);

		if (StaminaBottom)
		{
			if (Stamina->Percent > StaminaBottom->Percent)
			{
				bCanChangeStaminaBottom = true;
			}
			if (Stamina->Percent < StaminaBottom->Percent && bCanStaminaChangedTimerSet)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaBottomDelay, ChangeStaminaDelegate, 0.2f, false);
				bCanStaminaChangedTimerSet = false;
			}
		}
	}
}

void UARPGHUDWidget::SetMaxMana(float InMaxMana)
{
	ManaBox->SetWidthOverride(InMaxMana);
	MaxMana = InMaxMana;
}

void UARPGHUDWidget::SetCurrentMana(float CurrentMana)
{
	SetManaPercentage(CurrentMana / MaxMana);
	
	ManaPivotBox->SetWidthOverride(CurrentMana);
}

void UARPGHUDWidget::SetManaPercentage(float ManaPercentage)
{
	if (Mana)
	{
		Mana->SetPercent(ManaPercentage);

		if (ManaBottom)
		{
			if (Mana->Percent > ManaBottom->Percent)
			{
				bCanChangeManaBottom = true;
			}
			if (Mana->Percent < ManaBottom->Percent && bCanManaChangedTimerSet)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_ManaBottomDelay, ChangeManaDelegate, ChangeBottomDelay, false);
				bCanManaChangedTimerSet = false;
			}
		}
	}
}

void UARPGHUDWidget::SetMaxPosture(float InMaxPosture)
{
	// PostureBox->SetWidthOverride(InMaxPosture * 10.f);
	MaxPosture = InMaxPosture;
}

void UARPGHUDWidget::SetCurrentPosture(float CurrentPosture)
{
	if (CurrentPosture >= MaxPosture && PostureBar->WidthOverride > 0.f)
	{
		// PostureBox->SetVisibility(ESlateVisibility::Hidden);
		HidePostureBar();
	}
	else if (CurrentPosture < MaxPosture && PostureBar->WidthOverride == 0.f)
	{
		// PostureBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ShowPostureBar();
	}
	SetPosturePercentage(CurrentPosture / MaxPosture);
}

void UARPGHUDWidget::SetPosturePercentage(float PosturePercentage)
{
	PostureBar->SetWidthOverride(PostureBox->WidthOverride * (1 - PosturePercentage));
}

void UARPGHUDWidget::SetInteractText(const FText& InText)
{
	InteractText = InText;
	if (InText.IsEmpty())
	{
		InteractBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InteractBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UARPGHUDWidget::SetMessageText(const FText& InText, float Duration)
{
	MessageText = InText;
	if (!InText.IsEmpty())
	{
		MessageBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ClearMessageElapsed");
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_MessageClearDelay, Delegate, Duration, false);
	}
}

void UARPGHUDWidget::ShowMainMessage(const FText& Message, const FColor& Color, float Duration)
{
	if (!Message.IsEmpty())
	{
		MainMessage->SetText(Message);
		MainMessage->SetColorAndOpacity(FSlateColor(Color));
		ShowMainMessageAnimation();
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ClearMainMessageElapsed");
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_MainMessageClearDelay, Delegate, Duration, false);
	}
}

void UARPGHUDWidget::ClearMainMessageElapsed()
{
	HideMainMessageAnimation();
}

void UARPGHUDWidget::ShowMainMessageAnimation_Implementation() {}

void UARPGHUDWidget::HideMainMessageAnimation_Implementation() {}

void UARPGHUDWidget::ChangeHealthBottomElapsed()
{
	bCanChangeHealthBottom = true;
}

void UARPGHUDWidget::ChangeManaBottomElapsed()
{
	bCanChangeManaBottom = true;
}

void UARPGHUDWidget::ChangeStaminaBottomElapsed()
{
	bCanChangeStaminaBottom = true;
}

void UARPGHUDWidget::ClearMessageElapsed()
{
	MessageBox->SetVisibility(ESlateVisibility::Hidden);
	MessageText = FText();
}
