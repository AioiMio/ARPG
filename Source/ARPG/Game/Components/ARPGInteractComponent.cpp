// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGInteractComponent.h"

#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPG/Game/Core/ARPGPlayerController.h"
#include "ARPG/Game/Interface/ARPGInteractInterface.h"
#include "ARPG/Game/UI/ARPGHUDWidget.h"

UARPGInteractComponent::UARPGInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARPGInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPGCharacter>(GetOwner());
}

void UARPGInteractComponent::TickComponent(float DeltaTime,
                                           ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AARPGPlayerController* PC = Cast<AARPGPlayerController>(OwnerCharacter->GetController()))
	{
		if (UARPGHUDWidget* HUD = PC->GetHUD())
		{
			if (InteractTargets.Num() > 0)
			{
				if (InteractTargets[0] != LastInteractTarget && OwnerCharacter.IsValid())
				{
					LastInteractTarget = InteractTargets[0];
					
					if (InteractTargets[0] != nullptr)
					{
						if (IARPGInteractInterface::Execute_CanInteract(InteractTargets[0], OwnerCharacter.Get()))
						{
							HUD->SetInteractText(
								IARPGInteractInterface::Execute_GetInteractText(InteractTargets[0], OwnerCharacter.Get()));
						}
						else
						{
							// AActor* Temp = InteractTargets[0];
							// InteractTargets.RemoveAtSwap(0);
							// InteractTargets.Emplace(Temp);
						}
					}
					else
					{
						HUD->SetInteractText(FText());
						InteractTargets.RemoveAtSwap(0);
					}
				}
			}
			else
			{
				HUD->SetInteractText(FText());
				LastInteractTarget = nullptr;
			}
		}
	}
}

void UARPGInteractComponent::Interact_Implementation()
{
	if (InteractTargets.Num() > 0)
	{
		if (InteractTargets[0] && OwnerCharacter.IsValid())
		{
			IARPGInteractInterface::Execute_Interact(InteractTargets[0], OwnerCharacter.Get());
		}
	}
}
