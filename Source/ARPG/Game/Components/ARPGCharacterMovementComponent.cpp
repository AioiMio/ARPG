// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "Net/UnrealNetwork.h"

UARPGCharacterMovementComponent::UARPGCharacterMovementComponent()
{
	SprintSpeed = 800.f;
	RunSpeed = 600.f;
	WalkSpeed = 150.f;
}

float UARPGCharacterMovementComponent::GetMaxSpeed() const
{
	AARPGCharacter* Owner = Cast<AARPGCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive())
	{
		return 0.0f;
	}

	if (Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"))))
	{
		return 0.0f;
	}

	if (bIsWalking)
	{
		return WalkSpeed;
	}
	
	if (RequestToStartSprinting)
	{
		return SprintSpeed;
	}

	if (RequestToStartRunning)
	{
		return RunSpeed;
	}

	return Owner->GetMoveSpeed();
}

void UARPGCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	RequestToStartRunning = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
	RequestToStartWalking = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;
}

FNetworkPredictionData_Client* UARPGCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UARPGCharacterMovementComponent* MutableThis = const_cast<UARPGCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FARPGNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UARPGCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UARPGCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UARPGCharacterMovementComponent::StartRunning()
{
	RequestToStartRunning = true;
}

void UARPGCharacterMovementComponent::StopRunning()
{
	RequestToStartRunning = false;
}

void UARPGCharacterMovementComponent::StartWalking()
{
	// RequestToStartWalking = true;
	bIsWalking = true;
}

void UARPGCharacterMovementComponent::ServerStartWalking_Implementation()
{
	StartWalking();
}

void UARPGCharacterMovementComponent::StopWalking()
{
	// RequestToStartWalking = false;
	bIsWalking = false;
}

void UARPGCharacterMovementComponent::ServerStopWalking_Implementation()
{
	StopWalking();
}

void UARPGCharacterMovementComponent::FARPGSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartWalking = false;
}

uint8 UARPGCharacterMovementComponent::FARPGSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartWalking)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UARPGCharacterMovementComponent::FARPGSavedMove::CanCombineWith(const FSavedMovePtr& NewMove,
	ACharacter* Character,
	float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FARPGSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartWalking != ((FARPGSavedMove*)&NewMove)->SavedRequestToStartWalking)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UARPGCharacterMovementComponent::FARPGSavedMove::SetMoveFor(ACharacter* Character,
	float InDeltaTime,
	FVector const& NewAccel,
	FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UARPGCharacterMovementComponent* CharacterMovement = Cast<UARPGCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartWalking = CharacterMovement->RequestToStartWalking;
	}
}

void UARPGCharacterMovementComponent::FARPGSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UARPGCharacterMovementComponent* CharacterMovement = Cast<UARPGCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UARPGCharacterMovementComponent::FARPGNetworkPredictionData_Client::FARPGNetworkPredictionData_Client(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UARPGCharacterMovementComponent::FARPGNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FARPGSavedMove());
}

void UARPGCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPGCharacterMovementComponent, bIsWalking);
}
