// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Interface/ARPGInteractInterface.h"
#include "GameFramework/Actor.h"
#include "ARPGMechanism.generated.h"

class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;

UCLASS()
class ARPG_API AARPGMechanism : public AActor, public IARPGInteractInterface
{
	GENERATED_BODY()
	
public:	
	AARPGMechanism();

	FText GetInteractText_Implementation(APawn* InstigatorPawn);
	void Interact_Implementation(APawn *InstigatorPawn);
	bool CanInteract_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Interact")
	FText InteractText;

	UPROPERTY(EditAnywhere, Category = "Interact")
	FText FailedMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bTriggered;
};
