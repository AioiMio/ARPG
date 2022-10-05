// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPGInteractComponent.generated.h"

class AARPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGInteractComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Unreliable)
	void Interact();

	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	TArray<AActor*> InteractTargets;

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;
	
	TWeakObjectPtr<AActor> LastInteractTarget;
};
