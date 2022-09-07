// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Actor.h"
#include "ARPGEquipmentBase.generated.h"

class UBoxComponent;
class UArrowComponent;
class USkeletalMeshComponent;
class UARPGGameplayAbility;
class AARPGCharacter;

UCLASS()
class ARPG_API AARPGEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AARPGEquipmentBase();

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return SkeletalMeshComponent; }

	// Equipment abilities for owner Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UARPGGameplayAbility>> Abilities;

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* Icon;

	bool bEquipmentAbilitiesGiven = false;

	UFUNCTION(BlueprintImplementableEvent)
	void JoinWorld();

	UFUNCTION(BlueprintImplementableEvent)
	void LeaveWorld();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLeaveWorld();

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
