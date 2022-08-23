// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ARPGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UARPGAssetManager& Get();

	/** Starts initial load, gets called from InitializeObjectReferences **/
	virtual void StartInitialLoading() override;
};
