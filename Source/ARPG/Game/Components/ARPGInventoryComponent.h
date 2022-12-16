// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPGInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGInventoryComponent();

protected:
	virtual void BeginPlay() override;
};
