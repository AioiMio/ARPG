// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBTService_CheckTargetBreak.h"

#include "AbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARPGBTService_CheckTargetBreak::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check whether target actor is break
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent))
	{
		AARPGCharacter* TargetActor = Cast<AARPGCharacter>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			UAbilitySystemComponent* ASC = TargetActor->GetAbilitySystemComponent();
			if (ASC)
			{
				BlackboardComponent->SetValueAsBool(TargetBreakKey.SelectedKeyName, ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Break"))));
			}
		}
	}
}
