// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTTask_UpdatePatrolIndex.h"

#include "AIController.h"
#include "ARPG/Game/Enemies/ARPGEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UARPGBTTask_UpdatePatrolIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			AARPGEnemyCharacter* MyCharacter = Cast<AARPGEnemyCharacter>(MyController->GetPawn());
			if (MyCharacter == nullptr)
			{
				return EBTNodeResult::Failed;
			}

			int8 Index = BlackboardComponent->GetValueAsInt(PatrolIndex.SelectedKeyName);
			if (MyCharacter->PatrolPath.Num() > 0)
			{
				BlackboardComponent->SetValueAsInt(PatrolIndex.SelectedKeyName, (Index + 1) % (MyCharacter->PatrolPath.Num() + 1));
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
