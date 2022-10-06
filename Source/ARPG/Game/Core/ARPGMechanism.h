// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn);
	FText GetFailedMessage_Implementation(APawn* InstigatorPawn);
	bool Interact_Implementation(APawn *InstigatorPawn);
	bool CanInteract_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* AdditiveMesh;

	UPROPERTY(Replicated, EditAnywhere, Category = "Interact")
	FText InteractText;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText FailedMessage;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	FGameplayTag RequiredTag;
};
