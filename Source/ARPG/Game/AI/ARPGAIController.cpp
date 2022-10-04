// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

void AARPGAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsVector(FName("DefaultLocation"), GetPawn()->GetActorLocation());
	}
}
