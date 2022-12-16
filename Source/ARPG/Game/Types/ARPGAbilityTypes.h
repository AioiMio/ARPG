// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ARPG/Game/Components/ARPGCombatManager.h"
#include "ARPGAbilityTypes.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FARPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FARPGDamageData DamageData;

public:
	FARPGDamageData GetDamageData() const { return DamageData; }
	void SetDamageData(const FARPGDamageData InDamageData) { DamageData = InDamageData; }
	

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FARPGGameplayEffectContext* Duplicate() const override
	{
		FARPGGameplayEffectContext* NewContext = new FARPGGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
};

template <>
struct TStructOpsTypeTraits<FARPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FARPGGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
