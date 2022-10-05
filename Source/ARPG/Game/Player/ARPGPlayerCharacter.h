// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "ARPG/Game/Core/ARPGCharacter.h"
#include "ARPGPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UARPGInteractComponent;
class USphereComponent;

/**
 * 
 */
UCLASS()
class ARPG_API AARPGPlayerCharacter : public AARPGCharacter
{
	GENERATED_BODY()

public:
	AARPGPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Dying() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UARPGInteractComponent* GetInteractComponent() const { return InteractComponent; }

	UPROPERTY(Replicated)
	FVector2D MovementInputVector;

protected:
	virtual void BeginPlay() override;

	bool ASCInputBound = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	UARPGInteractComponent* InteractComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	USphereComponent* InteractSphere;

	void MovementInputBegin();
	void MovementInput(const FInputActionValue& InputActionValue);
	void MovementInputEnd();
	void Interact();

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();
};
