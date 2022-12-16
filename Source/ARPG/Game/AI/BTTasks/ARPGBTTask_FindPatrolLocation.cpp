// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTTask_FindPatrolLocation.h"

#include "AIController.h"
#include "ARPG/Game/Enemies/ARPGEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UARPGBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			if (MyCharacter->PatrolPath.Num() > 0 && Index < MyCharacter->PatrolPath.Num())
			{
				FVector PatrolPoint = MyCharacter->DefaultTransform.GetLocation() + MyCharacter->DefaultTransform.GetRotation().RotateVector(MyCharacter->PatrolPath[Index]);
				BlackboardComponent->SetValueAsVector(PatrolLocation.SelectedKeyName, PatrolPoint);
			}
			else
			{
				BlackboardComponent->SetValueAsVector(PatrolLocation.SelectedKeyName, MyCharacter->DefaultTransform.GetLocation());
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
