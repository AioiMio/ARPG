// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameModeBase.generated.h"

class AARPGPlayerCharacter;

/**
 * 
 */
UCLASS()
class ARPG_API AARPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARPGGameModeBase();

	void PlayerCharacterDied(AController* Controller);

protected:
	float RespawnDelay;

	TSubclassOf<AARPGPlayerCharacter> PlayerClass;

	AActor* EnemySpawnPoint;

	virtual void BeginPlay() override;

	void RespawnPlayerCharacter(AController* Controller);
	
};
