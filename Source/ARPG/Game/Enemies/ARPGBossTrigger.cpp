// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGBossTrigger.h"

#include "ARPGBossCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "ARPG/Game/UI/ARPGHealthBarWidget.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AARPGBossTrigger::AARPGBossTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	bTriggered = false;
}

void AARPGBossTrigger::ResetTrigger()
{
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
		if (PC && PC->GetHUD() && !bTriggered)
		{
			PC->GetHUD()->BossHealthBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (HasAuthority() && !bTriggered)
		{
			FTransform SpawnTransform = GetActorTransform();
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Z -= BoxComponent->GetScaledBoxExtent().Z / 2.f;
			SpawnTransform.SetLocation(SpawnLocation);
			SpawnTransform.SetScale3D(SpawnScale);
			
			AARPGBossCharacter* Boss = GetWorld()->SpawnActorDeferred<AARPGBossCharacter>(
				BossClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Boss->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			Boss->Trigger = this;
			Boss->FinishSpawning(SpawnTransform);
			Boss->OnCharacterDied.AddDynamic(this, &AARPGBossTrigger::OnBossDied);
			bTriggered = true;
		}
	}
}

void AARPGBossTrigger::OnBossDied(AARPGCharacter* Character)
{
	FogGate->Destroy();
}

void AARPGBossTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AARPGBossTrigger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPGBossTrigger, bTriggered);
}