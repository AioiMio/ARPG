// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Core/ARPGDrop.h"
#include "Components/ActorComponent.h"
#include "ARPGInteractComponent.generated.h"

class UARPGHUDWidget;
class AARPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGInteractComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	void Interact();
	UFUNCTION(Server, Unreliable)
	void ServerInteract(AActor* Target);

	void ShowFailedMessage(int32 Index);
	UFUNCTION(Client, Unreliable)
	void ClientShowFailedMessage(int32 Index);

	void Pickup(AARPGDrop* DropActor);

	void ShowItemName(AARPGDrop* DropActor);

	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	TArray<AActor*> InteractTargets;

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;
	TWeakObjectPtr<UARPGHUDWidget> HUD;

	UPROPERTY()
	AActor* CurrentInteractTarget;

	bool bInteractableActorFound;
	bool bInteractTargetChanged;

	void FindBestInteractableActor();
};
