// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ChangeMovementMode.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotify_ChangeMovementMode::UAnimNotify_ChangeMovementMode()
{
}

void UAnimNotify_ChangeMovementMode::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOuter());
	if (Character)
	{
		Character->GetCharacterMovement()->SetMovementMode(MovementMode);
	}
}
