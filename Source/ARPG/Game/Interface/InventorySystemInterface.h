// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventorySystemInterface.generated.h"

class UInventoryComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventorySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IInventorySystemInterface
{
	GENERATED_BODY()

public:
	/** Returns the inventory component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UInventoryComponent* GetInventoryComponent() const = 0;
};
