// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGProjectile.h"

#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AARPGProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AARPGProjectile::OnProjectileHit);
}

void AARPGProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Range / ProjectileMovement->InitialSpeed);
}

void AARPGProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}

	AARPGCharacter* VictimCharacter = Cast<AARPGCharacter>(OtherActor);
	if (HasAuthority() && VictimCharacter)
	{
		if (UARPGAbilitySystemComponent* ASC = VictimCharacter->GetARPGAbilitySystemComponent())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}
