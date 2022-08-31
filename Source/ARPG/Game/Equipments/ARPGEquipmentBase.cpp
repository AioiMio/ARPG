// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEquipmentBase.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


AARPGEquipmentBase::AARPGEquipmentBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void AARPGEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
}
