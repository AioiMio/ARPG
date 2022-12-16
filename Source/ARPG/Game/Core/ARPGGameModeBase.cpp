// Copyright Epic Games, Inc. All Rights Reserved.


#include "ARPGGameModeBase.h"

#include "ARPGPlayerController.h"
#include "EngineUtils.h"
#include "ARPG/Game/Enemies/ARPGBossCharacter.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
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

void AARPGGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	AActor* StartSpot = FindPlayerStart(NewPlayer, PlayerStartName.ToString());

	// If a start spot wasn't found,
	if (StartSpot == nullptr)
	{
		// Check for a previously assigned spot
		if (NewPlayer->StartSpot != nullptr)
		{
			StartSpot = NewPlayer->StartSpot.Get();
			UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
		}	
	}

	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
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

void AARPGGameModeBase::PlayerCharacterDied(AController* Controller)
{
	Controller->UnPossess();

	if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(Controller))
	{
		PC->ShowDiedMessage();
	}
	
	if (Controller == GetWorld()->GetFirstPlayerController())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (It->Get() != Controller)
			{
				AARPGPlayerController* PC = Cast<AARPGPlayerController>(It->Get());
				if (PC)
				{
					PC->ShowMessage(FText::FromString("Summoner has died. Returning to the last bonfire."), 5.f);
				}
			}
		}
	}

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &AARPGGameModeBase::RespawnPlayerCharacter, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

void AARPGGameModeBase::SetPlayerStartName(FName StartName)
{
	PlayerStartName = StartName;
}

void AARPGGameModeBase::RespawnPlayerCharacter(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		// DestroyAllBosses
		if (Controller->IsLocalController())
		{
			for (FActorIterator It(GetWorld()); It; ++It)
			{
				AARPGBossCharacter* Boss = Cast<AARPGBossCharacter>(*It);
				if (Boss)
				{
					if (Boss->Trigger.IsValid())
					{
						Boss->Trigger->ResetTrigger();
					}
					Boss->Destroy();
				}
			}

			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				if (It->Get() != Controller)
				{
					if (AARPGPlayerCharacter* InCharacter = Cast<AARPGPlayerCharacter>(It->Get()->GetPawn()))
					{
						if (InCharacter->IsAlive())
						{
							InCharacter->PreRestart();
							It->Get()->UnPossess();
							RespawnPlayerCharacter(It->Get());
						}
					}
				}
			}
		}

		AARPGPlayerController* PC = Cast<AARPGPlayerController>(Controller);
		if (PC->GetHUD())
		{
			PC->GetHUD()->BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		
		// Respawn player character
		AActor* PlayerStart = FindPlayerStart(Controller, PlayerStartName.ToString());

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
