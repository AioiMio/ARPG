// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGHealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UARPGHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bCanChangeHealthBottom = false;
	
	Damage = 0;
	DamageNum->SetVisibility(ESlateVisibility::Hidden);
}

void UARPGHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bCanChangeHealthBottom)
	{
		if (HealthTop->Percent >= HealthBottom->Percent)
		{
			bCanChangeHealthBottom = false;
		}
		else
		{
			float CurrentPercent = FMath::FInterpConstantTo(HealthBottom->Percent, HealthTop->Percent, InDeltaTime, 0.3f);
			HealthBottom->SetPercent(CurrentPercent);
		}
	}
}

void UARPGHealthBarWidget::SetHealthPercentage(float InPercent)
{
	if (HealthTop)
	{
		HealthTop->SetPercent(InPercent);
		
		if (HealthBottom)
		{
			if (HealthTop->Percent > HealthBottom->Percent)
			{
				HealthBottom->SetPercent(HealthTop->Percent);
			}
			if (HealthTop->Percent < HealthBottom->Percent)
			{
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ChangeHealthBottomElapsed");
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealthBottomDelay, Delegate, 0.8f, false);
			}
		}
	}
}

void UARPGHealthBarWidget::SetCharacterName(const FText& NewName)
{
	if (CharacterName)
	{
		CharacterName->SetText(NewName);
	}
}

void UARPGHealthBarWidget::ChangeHealthBottomElapsed()
{
	bCanChangeHealthBottom = true;
}

void UARPGHealthBarWidget::ShowDamageNumber(float ActualDelta)
{
	if (ActualDelta < 0.f)
	{
		Damage += -FMath::Floor(ActualDelta);
		DamageNum->SetText(FText::FromString(FString::FromInt(Damage)));
		DamageNum->SetVisibility(ESlateVisibility::Visible);

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "HideDamageNumber");
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideDamageNumberDelay, Delegate, 1.5f, false);
	}
}

void UARPGHealthBarWidget::HideDamageNumber()
{
	Damage = 0;
	DamageNum->SetVisibility(ESlateVisibility::Hidden);
}