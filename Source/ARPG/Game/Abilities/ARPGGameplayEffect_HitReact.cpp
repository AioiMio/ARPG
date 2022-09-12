// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayEffect_HitReact.h"

UARPGGameplayEffect_HitReact::UARPGGameplayEffect_HitReact()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(0.1f);
}
