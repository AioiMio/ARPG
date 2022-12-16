// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_MoveAndRotate.h"

#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


UAbilityTask_MoveAndRotate::UAbilityTask_MoveAndRotate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;
}

UAbilityTask_MoveAndRotate* UAbilityTask_MoveAndRotate::MoveAndRotate(UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	FVector Location,
	FRotator Rotation,
	float Duration,
	UCurveFloat* OptionalInterpolationCurve,
	UCurveVector* OptionalVectorInterpolationCurve)
{
	UAbilityTask_MoveAndRotate* MyObj = NewAbilityTask<UAbilityTask_MoveAndRotate>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
		MyObj->StartRotation = MyObj->GetAvatarActor()->GetActorRotation();
	}

	MyObj->TargetLocation = Location;
	MyObj->TargetRotation = Rotation;
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
	MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;
	MyObj->LerpCurve = OptionalInterpolationCurve;
	MyObj->LerpCurveVector = OptionalVectorInterpolationCurve;

	return MyObj;
}

void UAbilityTask_MoveAndRotate::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

	TimeMoveStarted = GetWorld()->GetTimeSeconds();
	TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
}

void UAbilityTask_MoveAndRotate::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(MOVE_Custom, 0);
			}
		}


		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			// Teleport in attempt to find a valid collision spot
			MyActor->TeleportTo(TargetLocation, TargetRotation);
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnTargetReached.Broadcast();
				}
				EndTask();
			}
		}
		else
		{
			FVector NewLocation;
			FRotator NewRotation;

			float MoveFraction = (CurrentTime - TimeMoveStarted) / DurationOfMovement;
			if (LerpCurveVector)
			{
				const FVector ComponentInterpolationFraction = LerpCurveVector->GetVectorValue(MoveFraction);
				NewLocation = FMath::Lerp<FVector, FVector>(StartLocation, TargetLocation, ComponentInterpolationFraction);
				// NewRotation = FMath::Lerp<FRotator, FVector>(StartRotation, TargetRotation, ComponentInterpolationFraction);
			}
			else
			{
				if (LerpCurve)
				{
					MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
				}

				NewLocation = FMath::Lerp<FVector, float>(StartLocation, TargetLocation, MoveFraction);
				NewRotation = FMath::Lerp<FRotator, float>(StartRotation, TargetRotation, MoveFraction);
			}

			MyActor->SetActorLocation(NewLocation);
			MyActor->SetActorRotation(NewRotation);
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UAbilityTask_MoveAndRotate::OnDestroy(bool AbilityIsEnding)
{
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp && CharMoveComp->MovementMode == MOVE_Custom)
			{
				CharMoveComp->SetMovementMode(MOVE_Falling);
			}
		}
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UAbilityTask_MoveAndRotate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityTask_MoveAndRotate, StartRotation);
	DOREPLIFETIME(UAbilityTask_MoveAndRotate, TargetRotation);
	DOREPLIFETIME(UAbilityTask_MoveAndRotate, StartLocation);
	DOREPLIFETIME(UAbilityTask_MoveAndRotate, TargetLocation);
	DOREPLIFETIME(UAbilityTask_MoveAndRotate, DurationOfMovement);
	DOREPLIFETIME(UAbilityTask_MoveAndRotate, LerpCurve);
	DOREPLIFETIME(UAbilityTask_MoveAndRotate, LerpCurveVector);
}
