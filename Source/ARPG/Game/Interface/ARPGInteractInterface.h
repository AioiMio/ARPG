// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPGInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARPGInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IARPGInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetFailedMessage(APawn* InstigatorPawn, int32 Index = 0);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetFailedMessageIndex(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Interact(APawn *InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(APawn* InstigatorPawn);
};
