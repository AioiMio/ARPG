// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGMechanism.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

AARPGMechanism::AARPGMechanism()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetIsReplicated(true);
	BaseMesh->SetGenerateOverlapEvents(false);

	AdditiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveMesh"));
	AdditiveMesh->SetupAttachment(BaseMesh);
	AdditiveMesh->SetIsReplicated(true);
	AdditiveMesh->SetGenerateOverlapEvents(false);

	bReplicates = true;
}

FText AARPGMechanism::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return InteractText;
}

FText AARPGMechanism::GetFailedMessage_Implementation(APawn* InstigatorPawn)
{
	return FailedMessage;
}

bool AARPGMechanism::Interact_Implementation(APawn* InstigatorPawn)
{
	return false;
}

bool AARPGMechanism::CanInteract_Implementation(APawn* InstigatorPawn)
{
	return !bTriggered;
}

void AARPGMechanism::BeginPlay()
{
	Super::BeginPlay();
}

void AARPGMechanism::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPGMechanism, InteractText);
	DOREPLIFETIME(AARPGMechanism, FailedMessage);
	DOREPLIFETIME(AARPGMechanism, bTriggered);
}
