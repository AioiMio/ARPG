// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGHealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


void UARPGHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bCanHealthChangeTimerSet = true;
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
			bCanHealthChangeTimerSet = true;
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
		HealthPivotBox->SetWidthOverride(InPercent * 200.f);
		
		if (HealthBottom)
		{
			if (HealthTop->Percent > HealthBottom->Percent)
			{
				HealthBottom->SetPercent(HealthTop->Percent);
			}
			if (HealthTop->Percent < HealthBottom->Percent && bCanHealthChangeTimerSet)
			{
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ChangeHealthBottomElapsed");
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealthBottomDelay, Delegate, ChangeHealthBottomDelay, false);
				bCanHealthChangeTimerSet = false;
			}
		}
	}
}

void UARPGHealthBarWidget::SetPosturePercentage(float InPercent)
{
	if (PostureBar)
	{
		PostureBar->SetPercent(InPercent);
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