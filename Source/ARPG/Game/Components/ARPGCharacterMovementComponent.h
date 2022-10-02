// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARPGCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FARPGSavedMove : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		///@brief Resets all saved variables.
		virtual void Clear() override;

		///@brief Store input commands in the compressed flags.
		virtual uint8 GetCompressedFlags() const override;

		///@brief This is used to check whether or not two moves can be combined into one.
		///Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		///@brief Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		///@brief Sets variables on character movement component before making a predictive correction.
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Sprint
		uint8 SavedRequestToStartSprinting : 1;

		// Aim Down Sights
		uint8 SavedRequestToStartWalking : 1;
	};

	class FARPGNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
	public:
		FARPGNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		///@brief Allocates a new copy of our custom saved move
		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	UARPGCharacterMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Walk")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Run")
	float RunSpeed;
	
	uint8 RequestToStartSprinting : 1;
	uint8 RequestToStartRunning : 1;
	uint8 RequestToStartWalking : 1;
	
	virtual float GetMaxSpeed() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Movement|Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Movement|Sprint")
	void StopSprinting();

	// Run
	UFUNCTION(BlueprintCallable, Category = "Movement|Run")
	void StartRunning();
	UFUNCTION(BlueprintCallable, Category = "Movement|Run")
	void StopRunning();

	// Walk
	UFUNCTION(BlueprintCallable, Category = "Movement|Walk")
	void StartWalking();
	UFUNCTION(BlueprintCallable, Category = "Movement|Walk")
	void StopWalking();
};
