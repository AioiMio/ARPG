// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGWeapon.h"

#include "AbilitySystemComponent.h"
#include "ARPG/Game/Abilities/ARPGGameplayAbility.h"
#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Net/UnrealNetwork.h"

AARPGWeapon::AARPGWeapon()
{
}

void AARPGWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPGWeapon, EquipPostion);
}

void AARPGWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (OwnerCharacter.IsValid() && !HasAuthority())
	{
		if (EquipPostion == EEquipPostion::RightHand)
		{
			OwnerCharacter->GetEquipmentManager()->SetCurrentRightHandWeapon(this);
		}
	}
}
