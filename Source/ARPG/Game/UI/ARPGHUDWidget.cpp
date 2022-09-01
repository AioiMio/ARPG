// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"

UARPGHUDWidget::UARPGHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCanChangeHealthBottom = false;
	bCanChangeManaBottom = false;
	bCanChangeStaminaBottom = false;

	MaxHealth = 0.f;
	MaxMana = 0.f;
	MaxStamina = 0.f;
}

void UARPGHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bCanChangeHealthBottom)
	{
		if (Health->Percent >= HealthBottom->Percent)
		{
			bCanChangeHealthBottom = false;
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
		}
		else
		{
			float CurrentPercent = FMath::FInterpConstantTo(StaminaBottom->Percent, Stamina->Percent, InDeltaTime, 0.3f);
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
				HealthBottom->SetPercent(Health->Percent);
			}
			if (Health->Percent < HealthBottom->Percent)
			{
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ChangeHealthBottomElapsed");
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealthBottomDelay, Delegate, 0.8f, false);
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
				StaminaBottom->SetPercent(Stamina->Percent);
			}
			if (Stamina->Percent < StaminaBottom->Percent)
			{
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ChangeStaminaBottomElapsed");
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaBottomDelay, Delegate, 0.8f, false);
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
				ManaBottom->SetPercent(Mana->Percent);
			}
			if (Mana->Percent < ManaBottom->Percent)
			{
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ChangeManaBottomElapsed");
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_ManaBottomDelay, Delegate, 0.8f, false);
			}
		}
	}

}

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
