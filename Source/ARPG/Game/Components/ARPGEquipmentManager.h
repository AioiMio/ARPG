// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Equipments/Weapons/ARPGWeapon.h"
#include "Components/ActorComponent.h"
#include "ARPGEquipmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponChangedDelegate, EEquipPostion, EquipPostion, AARPGWeapon*, NewWeapon);

class AARPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPGEquipmentManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARPGEquipmentManager();

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	UPROPERTY(BlueprintAssignable)
	FWeaponChangedDelegate WeaponChangedDelegate;

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerEquipRightHandWeapon(int32 Index);
	void EquipRightHandWeapon(int32 Index);
	void EquipRightHandWeaponByClass(TSubclassOf<AARPGWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerChangeRightHandWeapon();
	void ChangeRightHandWeapon();
	
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerDestroyRightHandWeapon();
	void DestroyRightHandWeapon();
	
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerEquipLeftHandWeapon(int32 Index);
	void EquipLeftHandWeapon(int32 Index);
	void EquipLeftHandWeaponByClass(TSubclassOf<AARPGWeapon> WeaponClass);
	
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerDestroyLeftHandWeapon();
	void DestroyLeftHandWeapon();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AARPGWeapon* GetCurrentRightHandWeapon() { return CurrentRightHandWeapon; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentRightHandWeapon(AARPGWeapon* Weapon) { CurrentRightHandWeapon = Weapon; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponType GetRightHandWeaponType() { return RightHandWeaponType; }
	FORCEINLINE void SetRightHandWeaponType(EWeaponType InWeaponType) { RightHandWeaponType = InWeaponType; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AARPGWeapon* GetCurrentLeftHandWeapon() { return CurrentLeftHandWeapon; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentLeftHandWeapon(AARPGWeapon* Weapon) { CurrentLeftHandWeapon = Weapon; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponType GetLeftHandWeaponType() { return LeftHandWeaponType; }
	FORCEINLINE void SetLeftHandWeaponType(EWeaponType InWeaponType) { LeftHandWeaponType = InWeaponType; }

	// Grant & Remove abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddEquipmentAbilitiesToOwner(AARPGEquipmentBase* Equipment);
	virtual void RemoveEquipmentAbilitiesFromOwner(AARPGEquipmentBase* Equipment);

	UFUNCTION(BlueprintCallable)
	void DestroyAllWeapons();
	void DissolveAllWeapons();
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipments")
	FName RightHandWeaponSocketName = "Weapon_R";
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipments")
	FName LeftHandWeaponSocketName = "Weapon_L";

	UFUNCTION()
	void OnWeaponChanged(EEquipPostion EquipPostion, AARPGWeapon* NewWeapon);

protected:
	virtual void BeginPlay() override;
	
	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AARPGWeapon> HandClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipments|Weapons")
	TArray<TSubclassOf<AARPGWeapon>> RightHandWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipments|Weapons")
	TArray<TSubclassOf<AARPGWeapon>> LeftHandWeapons;

	UPROPERTY(Replicated)
	AARPGWeapon* CurrentRightHandWeapon;
	UPROPERTY(Replicated)
	AARPGWeapon* CurrentLeftHandWeapon;

	uint8 CurrentRightHandWeaponSlotIndex;
	uint8 CurrentLeftHandWeaponSlotIndex;

	EWeaponType RightHandWeaponType;
	EWeaponType LeftHandWeaponType;
};
