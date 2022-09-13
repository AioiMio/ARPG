// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "ARPG/Game/Items/ARPGItem.h"

const FPrimaryAssetType UARPGAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType UARPGAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType UARPGAssetManager::EquipmentItemType = TEXT("Equipment");

UARPGAssetManager::UARPGAssetManager()
{
}

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

UARPGItem* UARPGAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UARPGItem* LoadedItem = Cast<UARPGItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
