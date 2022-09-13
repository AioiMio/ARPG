// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ARPGAssetManager.generated.h"

class UARPGItem;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UARPGAssetManager();
	
	static UARPGAssetManager& Get();

	/** Starts initial load, gets called from InitializeObjectReferences **/
	virtual void StartInitialLoading() override;

	/** Static types for items **/
	static const FPrimaryAssetType PotionItemType;
	static const FPrimaryAssetType TokenItemType;
	static const FPrimaryAssetType EquipmentItemType;

	UARPGItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
