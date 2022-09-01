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
	if (OwnerCharacter.IsValid() && RightHandWeapons[Index])
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AARPGWeapon* Weapon = GetWorld()->SpawnActor<AARPGWeapon>(RightHandWeapons[Index].Get(),
		                                                          OwnerCharacter->GetMesh()->GetSocketTransform(
			                                                          RightHandWeaponSocketName), ActorSpawnParameters);

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		Weapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentTransformRules, RightHandWeaponSocketName);
		Weapon->SetOwner(OwnerCharacter.Get());

		if (OwnerCharacter->GetCombatManager())
		{
			OwnerCharacter->GetCombatManager()->AddTraceMesh(Weapon->GetMesh(), EAGR_CombatColliderType::ComplexBoxTrace);
		}
	}
}

void UARPGEquipmentComponent::EquipLeftHandWeapon(int32 Index)
{
}
