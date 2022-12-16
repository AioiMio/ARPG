// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGDrop.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"

AARPGDrop::AARPGDrop()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(RootComponent);

	bReplicates = false;
}

void AARPGDrop::BeginPlay()
{
	Super::BeginPlay();
}

FText AARPGDrop::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return InteractText;
}

FText AARPGDrop::GetFailedMessage_Implementation(APawn* InstigatorPawn, int32 Index)
{
	return FailedMessages[Index];
}

int32 AARPGDrop::GetFailedMessageIndex_Implementation(APawn* InstigatorPawn)
{
	return FailedMessageIndex;
}

bool AARPGDrop::Interact_Implementation(APawn* InstigatorPawn)
{
	return false;
}

bool AARPGDrop::CanInteract_Implementation(APawn* InstigatorPawn)
{
	return !bPicked;
}
