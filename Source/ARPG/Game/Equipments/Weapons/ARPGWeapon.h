// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Equipments/ARPGEquipmentBase.h"
#include "ARPG/Game/Types/Types.h"
#include "ARPGWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Hand UMETA(DisplayName = "Hand"),
	Katana UMETA(DisplayName = "Katana"),
	Sword UMETA(DisplayName = "Sword"),
	GreatSword UMETA(DisplayName = "GreatSword"),
	
	MAX UMETA(DisplayName = "DefaultMAX")
};

class AARPGCharacter;

/**
 * 
 */
UCLASS()
class ARPG_API AARPGWeapon : public AARPGEquipmentBase
{
	GENERATED_BODY()

public:
	AARPGWeapon();

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEquipPosition(EEquipPostion Position) { EquipPostion = Position; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	EWeaponType WeaponType;
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment", Replicated)
	EEquipPostion EquipPostion;
};
