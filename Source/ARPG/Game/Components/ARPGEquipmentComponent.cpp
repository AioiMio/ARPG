// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEquipmentComponent.h"

#include "ARPGCombatManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"


UARPGEquipmentComponent::UARPGEquipmentComponent()
{
	RightHandWeapons.SetNum(3);
	LeftHandWeapons.SetNum(3);
}

void UARPGEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPGCharacter>(GetOwner());
	
	EquipRightHandWeapon(0);
}

void UARPGEquipmentComponent::EquipRightHandWeapon(int32 Index)
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->HasAuthority() && RightHandWeapons[Index])
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

void UARPGEquipmentComponent::EquipLeftHandWeapon(int32 Index)
{
}

void UARPGEquipmentComponent::UnequipAllWeapons()
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->HasAuthority())
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
