// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGInventory.h"
#include "UObject/NoExportTypes.h"
#include "ARPGItem.generated.h"

class UARPGInventory;

/**
 * 
 */
UCLASS()
class ARPG_API UARPGItem : public UObject
{
	GENERATED_BODY()

public:
	UARPGItem();

	FORCEINLINE virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	void SetOwnerInventory(UARPGInventory* Owner);
	void SetGuid(const FGuid& InGuid);
	void SetItemName(const FName& InItemName);
	void SetCount(int32 InCount);
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_OwnerInventory)
	UARPGInventory* OwnerInventory;

	UPROPERTY(Replicated)
	FGuid Guid;

	UPROPERTY(Replicated)
	FName ItemName;

	UPROPERTY(ReplicatedUsing = OnRep_Count)
	int32 Count;

	UFUNCTION()
	virtual void OnRep_OwnerInventory(const UARPGInventory* OldOwnerInventory);

	UFUNCTION()
	virtual void OnRep_Count(const int32 OldCount);
};
