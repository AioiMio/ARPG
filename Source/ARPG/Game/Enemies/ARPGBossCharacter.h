// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGBossTrigger.h"
#include "ARPGEnemyCharacter.h"
#include "ARPGBossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPGBossCharacter : public AARPGEnemyCharacter
{
	GENERATED_BODY()

public:
	AARPGBossCharacter(const class FObjectInitializer& ObjectInitializer);

	TWeakObjectPtr<UARPGHealthBarWidget> BossHealthBar;

	TWeakObjectPtr<AARPGBossTrigger> Trigger;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	void InitializeBossHealthBar() const;
	
	void HealthChanged(const FOnAttributeChangeData& Data) override;
	void PostureChanged(const FOnAttributeChangeData& Data) override;
};