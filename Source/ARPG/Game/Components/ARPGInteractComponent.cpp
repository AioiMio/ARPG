// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGInteractComponent.h"

#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Interface/ARPGInteractInterface.h"
#include "ARPG/Game/Player/ARPGPlayerCharacter.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"
#include "Net/UnrealNetwork.h"

UARPGInteractComponent::UARPGInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARPGInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPGCharacter>(GetOwner());
	if (OwnerCharacter.IsValid())
	{
		AARPGPlayerController* PC = Cast<AARPGPlayerController>(OwnerCharacter->GetController());
		if (PC)
		{
			HUD = PC->GetHUD();
		}
	}
}

void UARPGInteractComponent::TickComponent(float DeltaTime,
                                           ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractableActor();
		
		AARPGPlayerController* PC = Cast<AARPGPlayerController>(MyPawn->GetController());
		if (PC->GetHUD())
		{
			if (CurrentInteractTarget)
			{
				PC->GetHUD()->SetInteractText(IARPGInteractInterface::Execute_GetInteractText(CurrentInteractTarget, MyPawn));
			}
			else
			{
				PC->GetHUD()->SetInteractText(FText());
			}
		}
	}
}

void UARPGInteractComponent::FindBestInteractableActor()
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (InteractTargets.Num() > 0)
	{
		for (int8 i = InteractTargets.Num() - 1; i >= 0; i--)
		{
			if (InteractTargets[i] == nullptr)
			{
				InteractTargets.RemoveAt(i);
			}
		}

		if (InteractTargets.Num() > 0)
		{
			InteractTargets.Sort([&](const AActor& A, const AActor& B)
			{
				return FVector::Distance(A.GetActorLocation(), GetOwner()->GetActorLocation()) <
					FVector::Distance(B.GetActorLocation(), GetOwner()->GetActorLocation());
			});

			for (AActor* Target : InteractTargets)
			{
				if (IARPGInteractInterface::Execute_CanInteract(Target, MyPawn))
				{
					CurrentInteractTarget = Target;
					return;
				}
			}
		}
	}
	CurrentInteractTarget = nullptr;
}

void UARPGInteractComponent::Interact()
{
	ServerInteract(CurrentInteractTarget);
}

void UARPGInteractComponent::ServerInteract_Implementation(AActor* Target)
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (Target && IARPGInteractInterface::Execute_CanInteract(Target, MyPawn))
	{
		if (!IARPGInteractInterface::Execute_Interact(Target, MyPawn))
		{
			ClientShowFailedMessage();
		}
	}
}

void UARPGInteractComponent::ShowFailedMessage() const
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn)
	{
		AARPGPlayerController* PC = Cast<AARPGPlayerController>(MyPawn->GetController());
		if (PC)
		{
			PC->GetHUD()->SetMessageText(IARPGInteractInterface::Execute_GetFailedMessage(CurrentInteractTarget, MyPawn));
		}
	}
}

void UARPGInteractComponent::ClientShowFailedMessage_Implementation()
{
	ShowFailedMessage();
}

void UARPGInteractComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
