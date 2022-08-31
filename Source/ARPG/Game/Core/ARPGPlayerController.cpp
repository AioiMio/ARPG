// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerController.h"

#include "AbilitySystemComponent.h"
#include "ARPGCharacter.h"
#include "ARPGPlayerState.h"
// #include "EnhancedInputSubsystems.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"

AARPGPlayerController::AARPGPlayerController()
{
	CameraPitchMin = -30.f;
	CameraPitchMax = 45.f;
}

void AARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMax = CameraPitchMax;
		PlayerCameraManager->ViewPitchMin = CameraPitchMin;
	}

	// if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	// {
	// 	EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
	// }
}


void AARPGPlayerController::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	AARPGPlayerState* PS = GetPlayerState<AARPGPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UARPGHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();
}

void AARPGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AARPGPlayerState* PS = GetPlayerState<AARPGPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AARPGPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();
}
