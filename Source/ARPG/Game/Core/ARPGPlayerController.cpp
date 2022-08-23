// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerController.h"

#include "AbilitySystemComponent.h"
#include "ARPGCharacter.h"

AARPGPlayerController::AARPGPlayerController()
{
}

void AARPGPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	
	if (AARPGCharacter* InCharacter = Cast<AARPGCharacter>(P))
	{
		InCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(InCharacter, InCharacter);
	}
}
