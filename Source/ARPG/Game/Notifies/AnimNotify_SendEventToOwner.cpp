// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SendEventToOwner.h"

#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_SendEventToOwner::Notify(USkeletalMeshComponent* MeshComp,
                                          UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, EventPayload);
}
