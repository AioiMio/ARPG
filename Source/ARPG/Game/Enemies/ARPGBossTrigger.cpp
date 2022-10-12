// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBossTrigger.h"

#include "ARPGBossCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AARPGBossTrigger::AARPGBossTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	bTriggered = false;
}

void AARPGBossTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AARPGBossTrigger::OnBoxBeginOverlap);
}

void AARPGBossTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                         AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp,
                                         int32 OtherBodyIndex,
                                         bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (AARPGPlayerCharacter* Character = Cast<AARPGPlayerCharacter>(OtherActor))
	{
		AARPGPlayerController* PC = Cast<AARPGPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC && PC->GetHUD())
		{
			PC->GetHUD()->BossHealthBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (HasAuthority() && !bTriggered)
		{
			AARPGBossCharacter* Boss = GetWorld()->SpawnActorDeferred<AARPGBossCharacter>(
				BossClass, GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Boss->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			Boss->FinishSpawning(GetActorTransform());
			bTriggered = true;
		}
	}
}

void AARPGBossTrigger::BeginPlay()
{
	Super::BeginPlay();
}
