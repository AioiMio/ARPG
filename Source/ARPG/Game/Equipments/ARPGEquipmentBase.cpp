// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEquipmentBase.h"

#include "AbilitySystemComponent.h"
#include "ARPG/Game/Abilities/ARPGGameplayAbility.h"
#include "ARPG/Game/Components/ARPGEquipmentManager.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


AARPGEquipmentBase::AARPGEquipmentBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void AARPGEquipmentBase::MulticastLeaveWorld_Implementation()
{
	LeaveWorld();
}

void AARPGEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = GetOwner() ? Cast<AARPGCharacter>(GetOwner()) : nullptr;
	JoinWorld();
}
