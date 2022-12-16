// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPGBTTask_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS(Category = "GameplayAbility", meta = (DisplayName = "Activate Gameplay Ability"))
class ARPG_API UARPGBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UARPGBTTask_ActivateAbility(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient)
	UBehaviorTreeComponent* CachedOwnerComp;

	UPROPERTY(Transient)
	APawn* ControlledPawn;

	UPROPERTY(EditAnywhere, Category = "GameplayAbility")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	//~ Begin UBTTaskNode interface
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	                            uint8* NodeMemory,
	                            EBTNodeResult::Type TaskResult) override;

private:
	/** Clears off any delegates bound to gameplay task */
	void CleanUp(UBehaviorTreeComponent& OwnerComp);

	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* InAbility);

	UFUNCTION()
	void OnAbilityCancelled();

	FGameplayAbilitySpec GameplayAbilitySpec;
};
