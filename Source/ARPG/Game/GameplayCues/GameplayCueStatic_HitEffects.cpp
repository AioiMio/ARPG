// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueStatic_HitEffects.h"

bool UGameplayCueStatic_HitEffects::OnExecute_Implementation(AActor* MyTarget,
	const FGameplayCueParameters& Parameters) const
{
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
