// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Types/Types.h"
#include "GameFramework/PlayerController.h"
#include "ARPGPlayerController.generated.h"

class AARPGWeapon;
class UInputMappingContext;
class UARPGHUDWidget;

/**
 * 
 */
UCLASS()
class ARPG_API AARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AARPGPlayerController();

	void CreateHUD();

	FORCEINLINE UARPGHUDWidget* GetHUD() { return UIHUDWidget; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UARPGHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UARPGHUDWidget* UIHUDWidget;
	
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CameraPitchMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CameraPitchMax;
	
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
