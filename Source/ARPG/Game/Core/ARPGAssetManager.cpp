// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAssetManager.h"

#include "AbilitySystemGlobals.h"

UARPGAssetManager& UARPGAssetManager::Get()
{
	UARPGAssetManager* Singleton = Cast<UARPGAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be ARPGAssetManager!"));
		return *NewObject<UARPGAssetManager>();	 // never calls this
	}
}

void UARPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	UAbilitySystemGlobals::Get().InitGlobalData();
}
