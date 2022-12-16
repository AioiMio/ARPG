// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAbilitySystemLibrary.h"

#include "ComboGraphAbilityTypes.h"

void UARPGAbilitySystemLibrary::SetInstigator(FGameplayEffectContextHandle EffectContext, AActor* InInstigator, AActor* InEffectCauser)
{
	EffectContext.AddInstigator(InInstigator, InEffectCauser);
}

void UARPGAbilitySystemLibrary::SetOrigin(FGameplayEffectContextHandle EffectContext, FVector InOrigin)
{
	EffectContext.AddOrigin(InOrigin);
}

// FARPGDamageData UARPGAbilitySystemLibrary::EffectContextGetDamageData(
// 	FGameplayEffectContextHandle EffectContextHandle)
// {
// 	FComboGraphGameplayEffectContext* EffectContext = static_cast<FComboGraphGameplayEffectContext*>(EffectContextHandle.Get());
// 	
// 	if (EffectContext)
// 	{
// 		return EffectContext->GetDamageData();
// 	}
//
// 	return FARPGDamageData();
// }
//
// void UARPGAbilitySystemLibrary::EffectContextAddDamageData(FGameplayEffectContextHandle EffectContextHandle,
// 	const FARPGDamageData& DamageData)
// {
// 	FComboGraphGameplayEffectContext* EffectContext = static_cast<FComboGraphGameplayEffectContext*>(EffectContextHandle.Get());
//
// 	if (EffectContext)
// 	{
// 		EffectContext->SetDamageData(DamageData);
// 	}
// }
