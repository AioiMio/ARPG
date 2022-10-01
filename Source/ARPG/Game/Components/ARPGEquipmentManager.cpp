// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEquipmentManager.h"

#include "AbilitySystemComponent.h"
#include "ARPGCombatManager.h"
#include "ARPG/Game/Abilities/ARPGGameplayAbility.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Components/Image.h"
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
	WeaponChangedDelegate.AddDynamic(this, &UARPGEquipmentManager::OnWeaponChanged);

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

	if (OwnerCharacter.IsValid() && GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!RightHandWeapons[Index] || Index < 0 || Index > 2)
		{
			EquipRightHandWeaponByClass(HandClass);
			CurrentRightHandWeaponSlotIndex = Index;
			return;
		}

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

		// Broadcast WeaponChanged Event
		WeaponChangedDelegate.Broadcast(EEquipPostion::RightHand, CurrentRightHandWeapon);

		if (CurrentRightHandWeapon->GetWeaponType() == EWeaponType::Katana)
		{
			EquipLeftHandWeapon(0);
		}
		else
		{
			DestroyLeftHandWeapon();
		}

		// Add trace mesh
		if (OwnerCharacter->GetCombatManager())
		{
			OwnerCharacter->GetCombatManager()->AddTraceMesh(CurrentRightHandWeapon->GetMesh(),
			                                                 EAGR_CombatColliderType::ComplexBoxTrace);
		}
	}
}

void UARPGEquipmentManager::EquipRightHandWeaponByClass(TSubclassOf<AARPGWeapon> WeaponClass)
{
	if (CurrentRightHandWeapon)
	{
		DestroyRightHandWeapon();
	}

	if (OwnerCharacter.IsValid() && GetOwnerRole() == ENetRole::ROLE_Authority && WeaponClass)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParameters.Owner = OwnerCharacter.Get();
		CurrentRightHandWeapon = GetWorld()->SpawnActor<AARPGWeapon>(WeaponClass.Get(),
		                                                             OwnerCharacter->GetMesh()->GetSocketTransform(
			                                                             RightHandWeaponSocketName),
		                                                             ActorSpawnParameters);
		CurrentRightHandWeapon->SetEquipPosition(EEquipPostion::RightHand);
		RightHandWeaponType = CurrentRightHandWeapon->GetWeaponType();
		AddEquipmentAbilitiesToOwner(CurrentRightHandWeapon);

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		CurrentRightHandWeapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentTransformRules,
		                                          RightHandWeaponSocketName);

		// Broadcast WeaponChanged Event
		WeaponChangedDelegate.Broadcast(EEquipPostion::RightHand, CurrentRightHandWeapon);

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
	for (int8 i = 0; i < 2; i++)
	{
		NextIndex = (NextIndex + 1) % 3;
		if (RightHandWeapons[NextIndex] == nullptr || RightHandWeapons[NextIndex] == HandClass)
		{
			continue;
		}
		break;
	}

	EquipRightHandWeapon(NextIndex);
}

void UARPGEquipmentManager::EquipLeftHandWeapon(int32 Index)
{
	if (CurrentLeftHandWeapon)
	{
		DestroyLeftHandWeapon();
	}

	if (OwnerCharacter.IsValid() && GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!LeftHandWeapons[Index] || Index < 0 || Index > 2)
		{
			return;
		}

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParameters.Owner = OwnerCharacter.Get();
		CurrentLeftHandWeapon = GetWorld()->SpawnActor<AARPGWeapon>(LeftHandWeapons[Index].Get(),
																	 OwnerCharacter->GetMesh()->GetSocketTransform(
																		 LeftHandWeaponSocketName),
																	 ActorSpawnParameters);
		CurrentLeftHandWeapon->SetEquipPosition(EEquipPostion::LeftHand);
		CurrentLeftHandWeapon->SetActorHiddenInGame(true);
		CurrentLeftHandWeaponSlotIndex = Index;

		AddEquipmentAbilitiesToOwner(CurrentLeftHandWeapon);

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		CurrentLeftHandWeapon->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentTransformRules,
												  LeftHandWeaponSocketName);

		// Broadcast WeaponChanged Event
		WeaponChangedDelegate.Broadcast(EEquipPostion::LeftHand, CurrentLeftHandWeapon);
	}
}

void UARPGEquipmentManager::EquipLeftHandWeaponByClass(TSubclassOf<AARPGWeapon> WeaponClass)
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

void UARPGEquipmentManager::OnWeaponChanged(EEquipPostion EquipPostion, AARPGWeapon* NewWeapon)
{
	if (OwnerCharacter.IsValid() && OwnerCharacter->IsLocallyControlled())
	{
		AARPGPlayerController* PC = Cast<AARPGPlayerController>(OwnerCharacter->GetController());
		if (PC)
		{
			UARPGHUDWidget* HUD = PC->GetHUD();
			if (HUD)
			{
				if (EquipPostion == EEquipPostion::RightHand)
				{
					if (NewWeapon == nullptr || NewWeapon->GetWeaponType() == EWeaponType::Hand)
					{
						HUD->RightHandWeaponIcon->SetOpacity(0.f);
					}
					else
					{
						HUD->RightHandWeaponIcon->SetBrushFromTexture(NewWeapon->Icon);
						HUD->RightHandWeaponIcon->SetOpacity(1.f);
					}
				}
			}
		}
	}
}

void UARPGEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPGEquipmentManager, CurrentLeftHandWeapon);
	DOREPLIFETIME(UARPGEquipmentManager, CurrentRightHandWeapon);
}