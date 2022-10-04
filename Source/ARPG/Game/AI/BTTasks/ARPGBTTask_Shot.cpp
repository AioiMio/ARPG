// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTTask_Shot.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

EBTNodeResult::Type UARPGBTTask_Shot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AARPGCharacter* MyCharacter = Cast<AARPGCharacter>(MyController->GetPawn());
		if (MyCharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (UAbilitySystemComponent* ASC = MyCharacter->GetAbilitySystemComponent())
		{
			return ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Gun")))) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}
