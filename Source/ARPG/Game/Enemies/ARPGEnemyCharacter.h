// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPGEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPGEnemyCharacter : public AARPGCharacter
{
	GENERATED_BODY()

public:
	AARPGEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	
	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	UARPGAbilitySystemComponent* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSet
	UPROPERTY()
	UARPGAttributeSet* HardRefAttributeSet;

	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
