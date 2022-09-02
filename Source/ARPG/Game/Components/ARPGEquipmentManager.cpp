// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEquipmentManager.h"

#include "ARPGCombatManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"


UARPGEquipmentManager::UARPGEquipmentManager()
{
	RightHandWeapons.SetNum(3);
	LeftHandWeapons.SetNum(3);
}

void UARPGEquipmentManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPGCharacter>(GetOwner());
	
	EquipRightHandWeapon(0);
}

void UARPGEquipmentManager::EquipRightHandWeapon(int32 Index)
{
	if (OwnerCharacter.IsValid() && GetOwnerRole() == ENetRole::ROLE_Authority && RightHandWeapons[Index])
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentRightHandWeapon = GetWorld()->SpawnActor<AARPGWeapon>(RightHandWeapons[Index].Get(),
		                                                          OwnerCharacter->GetMesh()->GetSocketTransform(
			                                                          RightHandWeaponSocketName), ActorSpawnParameters);

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		CurrentRightHandWeapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentTransformRules, RightHandWeaponSocketName);
		CurrentRightHandWeapon->SetOwner(OwnerCharacter.Get());

		if (OwnerCharacter->GetCombatManager())
		{
			OwnerCharacter->GetCombatManager()->AddTraceMesh(CurrentRightHandWeapon->GetMesh(), EAGR_CombatColliderType::ComplexBoxTrace);
		}
	}
}

void UARPGEquipmentManager::EquipLeftHandWeapon(int32 Index)
{
}

void UARPGEquipmentManager::UnequipAllWeapons()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (CurrentLeftHandWeapon)
		{
			CurrentLeftHandWeapon->Destroy();
		}
		if (CurrentRightHandWeapon)
		{
			CurrentRightHandWeapon->Destroy();
		}
	}
}

void UARPGEquipmentManager::DissolveAllWeapons()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (CurrentLeftHandWeapon)
		{
			CurrentLeftHandWeapon->MulticastLeaveWorld();
		}
		if (CurrentRightHandWeapon)
		{
			CurrentRightHandWeapon->MulticastLeaveWorld();
		}
	}
}
