// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTTask_ActivateAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UARPGBTTask_ActivateAbility::UARPGBTTask_ActivateAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Activate Gameplay Ability";

	// instantiating to be able to register delegates
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UARPGBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!GameplayAbility)
	{
		UE_LOG(LogTemp, Error, TEXT("UARPGBTTask_ActivateAbility::ExecuteTask for %s (%s) has no valid gameplay ability defined."), *GetNameSafe(OwnerComp.GetAIOwner()), *GetNameSafe(ControlledPawn))
		return EBTNodeResult::Failed;
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn))
	{
		const UGameplayAbility* const InAbilityCDO = GameplayAbility.GetDefaultObject();
		
		for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
		{
			if (Spec.Ability == InAbilityCDO)
			{
				GameplayAbilitySpec = Spec;
				for (UGameplayAbility* Ability : GameplayAbilitySpec.GetAbilityInstances())
				{
					Ability->OnGameplayAbilityCancelled.AddUObject(this, &UARPGBTTask_ActivateAbility::OnAbilityCancelled);
					Ability->OnGameplayAbilityEnded.AddUObject(this, &UARPGBTTask_ActivateAbility::OnAbilityEnded);
				}
				
				return ASC->TryActivateAbility(Spec.Handle) ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("UARPGBTTask_ActivateAbility::ExecuteTask - %s owner has no Ability: %s."), *GetNameSafe(ControlledPawn), *GetNameSafe(GameplayAbility))
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UARPGBTTask_ActivateAbility::ExecuteTask - %s owner has no AbilitySystemComponent."), *GetNameSafe(ControlledPawn))
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UARPGBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CleanUp(OwnerComp);
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UARPGBTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	CleanUp(OwnerComp);
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UARPGBTTask_ActivateAbility::CleanUp(UBehaviorTreeComponent& OwnerComp)
{
	if (GameplayAbilitySpec.Ability)
	{
		for (UGameplayAbility* Ability : GameplayAbilitySpec.GetAbilityInstances())
		{
			Ability->OnGameplayAbilityCancelled.RemoveAll(this);
			Ability->OnGameplayAbilityEnded.RemoveAll(this);
		}
	}
}

void UARPGBTTask_ActivateAbility::OnAbilityEnded(UGameplayAbility* InAbility)
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

void UARPGBTTask_ActivateAbility::OnAbilityCancelled()
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Aborted);
}
