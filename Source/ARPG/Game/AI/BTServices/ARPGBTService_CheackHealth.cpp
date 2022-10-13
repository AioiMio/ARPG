// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTService_CheackHealth.h"

#include "AIController.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARPGBTService_CheackHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check actor's health
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			AARPGCharacter* AICharacter = Cast<AARPGCharacter>(MyController->GetPawn());
			if (AICharacter)
			{
				BlackboardComponent->SetValueAsFloat(HealthKey.SelectedKeyName, AICharacter->GetHealth() / AICharacter->GetMaxHealth());
			}
		}
	}
}
