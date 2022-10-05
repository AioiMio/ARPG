// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGMechanism.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AARPGMechanism::AARPGMechanism()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetIsReplicated(true);

	bReplicates = true;
}

FText AARPGMechanism::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return InteractText;
}

void AARPGMechanism::Interact_Implementation(APawn* InstigatorPawn)
{
}

bool AARPGMechanism::CanInteract_Implementation(APawn* InstigatorPawn)
{
	return true;
}

void AARPGMechanism::BeginPlay()
{
	Super::BeginPlay();
}

