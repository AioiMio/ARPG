// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTTask_VisceralAttack.h"

#include "AIController.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

EBTNodeResult::Type UARPGBTTask_VisceralAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AARPGCharacter* MyCharacter = Cast<AARPGCharacter>(MyController->GetPawn());
		if (MyCharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (UARPGCombatManager* CombatManager = MyCharacter->GetCombatManager())
		{
			return CombatManager->TryVisceralAttack(true) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}
