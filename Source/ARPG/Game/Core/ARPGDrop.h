// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Interface/ARPGInteractInterface.h"
#include "GameFramework/Actor.h"
#include "ARPGDrop.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class ARPG_API AARPGDrop : public AActor, public IARPGInteractInterface
{
	GENERATED_BODY()
	
public:	
	AARPGDrop();

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	FText GetFailedMessage_Implementation(APawn* InstigatorPawn) override;
	bool Interact_Implementation(APawn *InstigatorPawn) override;
	bool CanInteract_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Interact")
	FText InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText FailedMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bPicked;

};
