// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Equipments/ARPGEquipmentBase.h"
#include "ARPG/Game/Types/Types.h"
#include "ARPGWeapon.generated.h"

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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEquipPosition(EEquipPostion Position) { EquipPostion = Position; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment", Replicated)
	EEquipPostion EquipPostion;
};
