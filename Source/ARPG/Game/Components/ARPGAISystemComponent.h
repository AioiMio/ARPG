// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPGAISystemComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGAISystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGAISystemComponent();

protected:
	virtual void BeginPlay() override;
};
