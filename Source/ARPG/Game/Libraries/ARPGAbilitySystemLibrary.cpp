// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAbilitySystemLibrary.h"

void UARPGAbilitySystemLibrary::SetInstigator(FGameplayEffectContextHandle EffectContext, AActor* InInstigator, AActor* InEffectCauser)
{
	EffectContext.AddInstigator(InInstigator, InEffectCauser);
}

void UARPGAbilitySystemLibrary::SetOrigin(FGameplayEffectContextHandle EffectContext, FVector InOrigin)
{
	EffectContext.AddOrigin(InOrigin);
}
