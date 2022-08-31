// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGHealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"


void UARPGHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bCanChangeHealthBottom = false;
	
	Damage = 0;
	DamageNum->SetVisibility(ESlateVisibility::Hidden);
}

void UARPGHealthBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UARPGHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
}

void UARPGHealthBarWidget::SetHealthPercentage(float InPercent)
{
	if (Health)
	{
		Health->SetPercent(InPercent);
		
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

void UARPGHealthBarWidget::ChangeHealthBottomElapsed()
{
	bCanChangeHealthBottom = true;
}

void UARPGHealthBarWidget::ShowDamageNumber(float ActualDelta)
{
	if (ActualDelta < 0.f)
	{
		Damage += -FMath::Floor(ActualDelta);
		// DamageNum->SetText(FText::FromString(FString::FromInt(Damage)));
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


void UARPGHealthBarWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPGHealthBarWidget, Damage);
	DOREPLIFETIME(UARPGHealthBarWidget, DamageNum);
}