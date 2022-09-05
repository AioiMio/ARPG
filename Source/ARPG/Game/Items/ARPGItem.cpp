// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGItem.h"

#include "Net/UnrealNetwork.h"

UARPGItem::UARPGItem()
{
}

void UARPGItem::SetOwnerInventory(UARPGInventory* Owner)
{
	OwnerInventory = Owner;
	// MARK_PROPERTY_DIRTY_FROM_NAME(UARPGItem, OwnerInventory, this);
}

void UARPGItem::SetGuid(const FGuid& InGuid)
{
	Guid = InGuid;
	// MARK_PROPERTY_DIRTY_FROM_NAME(UARPGItem, Guid, this);
}

void UARPGItem::SetItemName(const FName& InItemName)
{
	ItemName = InItemName;
	// MARK_PROPERTY_DIRTY_FROM_NAME(UARPGItem, ItemName, this);
}

void UARPGItem::SetCount(int32 InCount)
{
	Count = InCount;
	// MARK_PROPERTY_DIRTY_FROM_NAME(UARPGItem, Count, this);
}

void UARPGItem::OnRep_OwnerInventory(const UARPGInventory* OldOwnerInventory)
{
}

void UARPGItem::OnRep_Count(const int32 OldCount)
{
}

void UARPGItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPGItem, Guid);
	DOREPLIFETIME(UARPGItem, ItemName);
	DOREPLIFETIME(UARPGItem, OwnerInventory);
	DOREPLIFETIME(UARPGItem, Count);
}

