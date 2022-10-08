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

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	FText GetFailedMessage_Implementation(APawn* InstigatorPawn, int32 Index) override;
	int32 GetFailedMessageIndex_Implementation(APawn* InstigatorPawn) override;
	bool Interact_Implementation(APawn *InstigatorPawn) override;
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

	UPROPERTY(EditAnywhere, Category = "Interact")
	FText InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TArray<FText> FailedMessages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	int32 FailedMessageIndex = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	FGameplayTag RequiredTag;
};
