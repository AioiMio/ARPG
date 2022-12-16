// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "AbilitySystemInterface.h"
#include "ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerState.h"


FARPGInventoryItem::FARPGInventoryItem(): ARPGItem(nullptr), ItemCount(0)
{
}

bool FARPGInventoryItem::IsValid()
{
	return ARPGItem && ItemCount > 0;
}

void FARPGInventoryItem::ResetSelf()
{
	ARPGItem = nullptr;
	ItemCount = 0;
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddInventoryItem(FARPGInventoryItem NewItem)
{
	if (GetOwner()->HasAuthority())
	{
		if (!NewItem.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
			return false;
		}
		
		bool bFind = false;
		for (FARPGInventoryItem& InventoryItem : InventoryItems)
		{
			if (InventoryItem.ARPGItem == NewItem.ARPGItem)
			{
				InventoryItem.ItemCount += NewItem.ItemCount;
				InventoryItemChanged(InventoryItems);
				return true;
			}
		}

		for (FARPGInventoryItem& InventoryItem : InventoryItems)
		{
			if (!InventoryItem.ARPGItem)
			{
				InventoryItem = NewItem;
				InventoryItemChanged(InventoryItems);
				return true;
			}
		}
	}
	return false;
}

void UInventoryComponent::RemoveInventoryItemByIndex(int32 Index_Remove)
{
	InventoryItems[Index_Remove].ResetSelf();
	InventoryItemChanged(InventoryItems);
}

void UInventoryComponent::RemoveInventoryItem(FARPGInventoryItem InItem, int32 Count)
{
	for (FARPGInventoryItem& InventoryItem : InventoryItems)
	{
		if (InventoryItem.ARPGItem == InItem.ARPGItem)
		{
			InventoryItem.ItemCount -= Count > 0 ? Count : InItem.ItemCount;
			if (InventoryItem.ItemCount < 0)
			{
				InventoryItem.ResetSelf();
			}
			InventoryItemChanged(InventoryItems);
			break;
		}
	}
}

void UInventoryComponent::SwapInventoryItem(int32 Index_A, int32 Index_B)
{
	InventoryItems.Swap(Index_A, Index_B);
	InventoryItemChanged(InventoryItems);
}

void UInventoryComponent::InventoryItemChanged_Implementation(const TArray<FARPGInventoryItem>& InInventoryItems)
{
	OnInventoryItemChanged.Broadcast(InInventoryItems);
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AARPGCharacter* InCharacter = Cast<AARPGCharacter>(GetOwner()))
	{
		OwnerCharacter = InCharacter;
	}
	else if (AARPGPlayerState* PS = Cast<AARPGPlayerState>(GetOwner()))
	{
		OwnerCharacter = Cast<AARPGCharacter>(PS->GetPawn());
	}

	if (OwnerCharacter.IsValid())
	{
		AbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
	}

	// Initialize inventory component
	if (InventoryItems.Num() == 0)
	{
		for (size_t i = 0; i < 30; i++)
		{
			InventoryItems.Add(FARPGInventoryItem());
		}
	}
	InventoryItemChanged(InventoryItems);
}
