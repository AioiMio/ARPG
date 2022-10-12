// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	virtual void Die() override;

	TWeakObjectPtr<UARPGHealthBarWidget> BossHealthBar;

protected:
	virtual void BeginPlay() override;
	
	void InitializeBossHealthBar() const;
	
	void HealthChanged(const FOnAttributeChangeData& Data) override;
	void PostureChanged(const FOnAttributeChangeData& Data) override;
};