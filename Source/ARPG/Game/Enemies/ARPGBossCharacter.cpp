// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBossCharacter.h"

#include "ARPG/Game/AI/ARPGAIController.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Components/SizeBox.h"
#include "Components/WidgetComponent.h"

AARPGBossCharacter::AARPGBossCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DiscoverWidgetComponent->SetVisibility(false);
	HealthBarComponent->SetVisibility(false);
}

void AARPGBossCharacter::InitializeBossHealthBar() const
{
	BossHealthBar->HealthPivotBox->SetVisibility(ESlateVisibility::Hidden);
	BossHealthBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
	BossHealthBar->SetPosturePercentage(GetPosture() / GetMaxPosture());

	BossHealthBar->SetCharacterName(CharacterName);
}

void AARPGBossCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	Super::HealthChanged(Data);

	float Health = Data.NewValue;
	float Damage = Health - Data.OldValue;

	if (BossHealthBar.IsValid())
	{
		BossHealthBar->SetHealthPercentage(Health / GetMaxHealth());
		BossHealthBar->ShowDamageNumber(Damage);
	}
}

void AARPGBossCharacter::PostureChanged(const FOnAttributeChangeData& Data)
{
	Super::PostureChanged(Data);

	float Posture = Data.NewValue;
	float Delta = Posture - Data.OldValue;

	if (BossHealthBar.IsValid())
	{
		BossHealthBar->SetPosturePercentage(GetPosture() / GetMaxPosture());
	}
}

void AARPGBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (PC && PC->GetHUD())
		{
			BossHealthBar = PC->GetHUD()->BossHealthBar;
			InitializeBossHealthBar();
		}
	}
}

void AARPGBossCharacter::Die()
{
	if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->ShowBossDestroyedMessage();
	}

	Super::Die();
}

void AARPGBossCharacter::Destroyed()
{
	Super::Destroyed();

	if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (PC->GetHUD())
		{
			PC->GetHUD()->BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
