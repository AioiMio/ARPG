// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEquipmentManager.h"

#include "AbilitySystemComponent.h"
#include "ARPGCombatManager.h"
#include "ARPG/Game/Abilities/ARPGGameplayAbility.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Net/UnrealNetwork.h"


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

void UARPGEquipmentManager::AddEquipmentAbilitiesToOwner(AARPGEquipmentBase* Equipment)
{
	if (Equipment && OwnerCharacter.IsValid())
	{
		// Grant abilities, but only on the server	
		if (GetOwnerRole() != ENetRole::ROLE_Authority || !OwnerCharacter->GetAbilitySystemComponent() || Equipment->
			bEquipmentAbilitiesGiven)
		{
			return;
		}

		for (TSubclassOf<UARPGGameplayAbility>& Ability : Equipment->Abilities)
		{
			FGameplayAbilitySpecHandle AbilitySpecHandle = OwnerCharacter->GetAbilitySystemComponent()->GiveAbility(
				FGameplayAbilitySpec(Ability, OwnerCharacter->GetAbilityLevel(Ability.GetDefaultObject()->AbilityID),
				                     static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID),
				                     this));

			Equipment->AbilitySpecHandles.Add(AbilitySpecHandle);
		}

		Equipment->bEquipmentAbilitiesGiven = true;
	}
}

void UARPGEquipmentManager::RemoveEquipmentAbilitiesFromOwner(AARPGEquipmentBase* Equipment)
{
	if (OwnerCharacter.IsValid())
	{
		// Remove abilities, but only on the server	
		if (OwnerCharacter->GetLocalRole() != ROLE_Authority || !OwnerCharacter->GetAbilitySystemComponent())
		{
			return;
		}

		for (FGameplayAbilitySpecHandle& AbilitySpecHandle : Equipment->AbilitySpecHandles)
		{
			OwnerCharacter->GetAbilitySystemComponent()->ClearAbility(AbilitySpecHandle);
		}
	}
}

void UARPGEquipmentManager::EquipRightHandWeapon(int32 Index)
{
	if (CurrentRightHandWeapon)
	{
		DestroyRightHandWeapon();
	}
	
	if (OwnerCharacter.IsValid() && GetOwnerRole() == ENetRole::ROLE_Authority && RightHandWeapons[Index])
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParameters.Owner = OwnerCharacter.Get();
		CurrentRightHandWeapon = GetWorld()->SpawnActor<AARPGWeapon>(RightHandWeapons[Index].Get(),
		                                                             OwnerCharacter->GetMesh()->GetSocketTransform(
			                                                             RightHandWeaponSocketName),
		                                                             ActorSpawnParameters);
		CurrentRightHandWeapon->SetEquipPosition(EEquipPostion::RightHand);
		CurrentRightHandWeaponSlotIndex = Index;
		RightHandWeaponType = CurrentRightHandWeapon->GetWeaponType();
		AddEquipmentAbilitiesToOwner(CurrentRightHandWeapon);

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		CurrentRightHandWeapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentTransformRules,
		                                          RightHandWeaponSocketName);

		// Add trace mesh
		if (OwnerCharacter->GetCombatManager())
		{
			OwnerCharacter->GetCombatManager()->AddTraceMesh(CurrentRightHandWeapon->GetMesh(),
			                                                 EAGR_CombatColliderType::ComplexBoxTrace);
		}
	}
}

void UARPGEquipmentManager::ServerChangeRightHandWeapon_Implementation()
{
	ChangeRightHandWeapon();
}

void UARPGEquipmentManager::ChangeRightHandWeapon()
{
	uint8 NextIndex = CurrentRightHandWeaponSlotIndex;
	for (int8 i = 0; i < 3; i++)
	{
		NextIndex = (NextIndex + 1) % 3;
		if (RightHandWeapons[NextIndex] == nullptr)
		{
			continue;
		}
		break;
	}
	
	if (RightHandWeapons[NextIndex])
	{
		EquipRightHandWeapon(NextIndex);
	}
}

void UARPGEquipmentManager::EquipLeftHandWeapon(int32 Index)
{
}

void UARPGEquipmentManager::DestroyRightHandWeapon()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority && CurrentRightHandWeapon)
	{
		// Remove trace mesh
		if (OwnerCharacter.IsValid() && OwnerCharacter->GetCombatManager())
		{
			OwnerCharacter->GetCombatManager()->RemoveTraceMesh(CurrentRightHandWeapon->GetMesh());
		}

		// Remove equipment abilities
		RemoveEquipmentAbilitiesFromOwner(CurrentRightHandWeapon);
		CurrentRightHandWeapon->Destroy();
	}
}

void UARPGEquipmentManager::DestroyLeftHandWeapon()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority && CurrentLeftHandWeapon)
	{
		RemoveEquipmentAbilitiesFromOwner(CurrentLeftHandWeapon);
		CurrentLeftHandWeapon->Destroy();
	}
}

void UARPGEquipmentManager::ServerEquipRightHandWeapon_Implementation(int32 Index)
{
	EquipRightHandWeapon(Index);
}

void UARPGEquipmentManager::ServerDestroyRightHandWeapon_Implementation()
{
	DestroyRightHandWeapon();
}

void UARPGEquipmentManager::ServerEquipLeftHandWeapon_Implementation(int32 Index)
{
	EquipLeftHandWeapon(Index);
}

void UARPGEquipmentManager::ServerDestroyLeftHandWeapon_Implementation()
{
	DestroyLeftHandWeapon();
}

void UARPGEquipmentManager::DestroyAllWeapons()
{
	DestroyRightHandWeapon();
	DestroyLeftHandWeapon();
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
