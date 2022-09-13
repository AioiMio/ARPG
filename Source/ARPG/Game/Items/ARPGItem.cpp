// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGItem.h"

UARPGItem::UARPGItem(): Price(0), MaxCount(1), MaxLevel(1), AbilityLevel(1)
{
}

bool UARPGItem::IsConsumable() const
{
	return MaxCount <= 0;
}

FString UARPGItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UARPGItem::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(ItemType, GetFName());
}
