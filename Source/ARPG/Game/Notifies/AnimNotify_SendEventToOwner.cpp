// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SendEventToOwner.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ARPG/Game/Core/ARPGCharacter.h"

void UAnimNotify_SendEventToOwner::Notify(USkeletalMeshComponent* MeshComp,
                                          UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AARPGCharacter* OwnerCharacter = Cast<AARPGCharacter>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerCharacter, EventTag, EventPayload);

			if (GameplayCueTag.IsValid())
			{
				FGameplayEffectContextHandle EffectContext;
				EffectContext.AddInstigator(OwnerCharacter, OwnerCharacter);
				ASC->ExecuteGameplayCue(GameplayCueTag, EffectContext);
			}
		}
	}
}
