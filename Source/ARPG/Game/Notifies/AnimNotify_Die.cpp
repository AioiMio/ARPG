// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Die.h"

#include "ARPG/Game/Core/ARPGCharacter.h"

void UAnimNotify_Die::Notify(USkeletalMeshComponent* MeshComp,
                             UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AARPGCharacter* Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
	if (Character && Character->HasAuthority())
	{
		Character->Dying();
	}
}
