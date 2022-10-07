// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPGDiscoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGDiscoverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void ShowDiscoverImage();
};
