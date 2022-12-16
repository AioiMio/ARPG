// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChangedNative, TArray<FARPGInventoryItem>);

class UARPGAbilitySystemComponent;
class AARPGCharacter;
class UARPGItem;

USTRUCT(BlueprintType)
struct FARPGInventoryItem
{
	GENERATED_BODY()

	FARPGInventoryItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPGInventoryItem")
	UARPGItem* ARPGItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARPGInventoryItem")
	int32 ItemCount;

	bool IsValid();

	void ResetSelf();
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddInventoryItem(FARPGInventoryItem NewItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveInventoryItemByIndex(int32 Index_Remove);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveInventoryItem(FARPGInventoryItem InItem, int32 Count = -1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapInventoryItem(int32 Index_A, int32 Index_B);

	// Refresh UI
	UFUNCTION(Client, Reliable)
	void InventoryItemChanged(const TArray<FARPGInventoryItem>& InInventoryItems);

	FOnInventoryItemChangedNative OnInventoryItemChanged;

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;

	TWeakObjectPtr<UARPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FARPGInventoryItem> InventoryItems;
};
