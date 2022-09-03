// Copyright Epic Games, Inc. All Rights Reserved.


#include "ARPGGameModeBase.h"

#include "ARPGPlayerController.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"

AARPGGameModeBase::AARPGGameModeBase()
{
	RespawnDelay = 5.0f;

	PlayerClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/_ARPG/Characters/PlayerCharacter_BP.PlayerCharacter_BP_C"));
	if (!PlayerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find PlayerClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
}

void AARPGGameModeBase::PlayerCharacterDied(AController* Controller)
{
	// FActorSpawnParameters SpawnParameters;
	// SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);
	//
	Controller->UnPossess();
	// Controller->Possess(SpectatorPawn);

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &AARPGGameModeBase::RespawnPlayerCharacter, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

void AARPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Get the enemy hero spawn point
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (Actor->GetName() == FString("EnemySpawn"))
		{
			EnemySpawnPoint = Actor;
			break;
		}
	}
}

void AARPGGameModeBase::RespawnPlayerCharacter(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		// Respawn player character
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AARPGPlayerCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<AARPGPlayerCharacter>(PlayerClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		// APawn* OldSpectatorPawn = Controller->GetPawn();
		// Controller->UnPossess();
		// OldSpectatorPawn->Destroy();
		Controller->Possess(NewPlayerCharacter);
	}
	else
	{
		// Respawn AI hero
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AARPGPlayerCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<AARPGPlayerCharacter>(PlayerClass, EnemySpawnPoint->GetActorTransform(), SpawnParameters);
		
		// APawn* OldSpectatorPawn = Controller->GetPawn();
		// Controller->UnPossess();
		// OldSpectatorPawn->Destroy();
		Controller->Possess(NewPlayerCharacter);
	}
}
