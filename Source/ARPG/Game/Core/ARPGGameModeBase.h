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

	virtual void RestartPlayer(AController* NewPlayer) override;
	
	void PlayerCharacterDied(AController* Controller);

	UFUNCTION(BlueprintCallable)
	void SetPlayerStartName(FName StartName);

protected:
	float RespawnDelay;

	TSubclassOf<AARPGPlayerCharacter> PlayerClass;

	AActor* EnemySpawnPoint;

	UPROPERTY(EditAnywhere)
	FName PlayerStartName;

	virtual void BeginPlay() override;

	void RespawnPlayerCharacter(AController* Controller);
	
};
