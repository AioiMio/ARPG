// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AddGameplayTags.h"

#include "AbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

void UAnimNotifyState_AddGameplayTags::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                   UAnimSequenceBase* Animation,
                                                   float TotalDuration,
                                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AARPGCharacter* Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
	if (Character)
	{
		if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
		{
			ASC->AddLooseGameplayTags(GameplayTags);
			bTagAdded = true;
		}
	}
}

void UAnimNotifyState_AddGameplayTags::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	AARPGCharacter* Character = Cast<AARPGCharacter>(MeshComp->GetOuter());
	if (Character)
	{
		UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
		if (ASC && bTagAdded)
		{
			ASC->RemoveLooseGameplayTags(GameplayTags);
		}
	}
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
