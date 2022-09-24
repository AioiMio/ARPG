// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGProjectile.h"

#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AARPGProjectile::AARPGProjectile()
{
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("Projectile"));

	Arrow = CreateDefaultSubobject<UArrowComponent>(FName("Arrow"));
	Arrow->SetupAttachment(BoxComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	Mesh->SetupAttachment(BoxComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AARPGProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AARPGProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Range / ProjectileMovement->InitialSpeed);
}
