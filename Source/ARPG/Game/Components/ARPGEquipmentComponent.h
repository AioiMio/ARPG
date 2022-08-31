// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Equipments/Weapons/ARPGWeapon.h"
#include "Components/ActorComponent.h"
#include "ARPGEquipmentComponent.generated.h"

class AARPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGEquipmentComponent();

	void EquipRightHandWeapon(int32 Index);
	void EquipLeftHandWeapon(int32 Index);

protected:
	virtual void BeginPlay() override;
	
	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipments|Weapons")
	TArray<TSubclassOf<AARPGWeapon>> RightHandWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipments|Weapons")
	TArray<TSubclassOf<AARPGWeapon>> LeftHandWeapons;

	UPROPERTY(EditDefaultsOnly, Category = "Equipments")
	FName RightHandWeaponSocketName = "Weapon_R";
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipments")
	FName LeftHandWeaponSocketName = "Weapon_L";
};
