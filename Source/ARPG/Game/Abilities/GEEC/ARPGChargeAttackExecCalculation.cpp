// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGChargeAttackExecCalculation.h"
#include "ARPG/Game/Components/ARPGAbilitySystemComponent.h"
#include "ARPG/Game/Components/ARPGAttributeSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct ARPGChargeDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageNegation);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Posture);

	ARPGChargeDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture the Source's AttackPower. Snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGAttributeSet, AttackPower, Source, true);

		// Capture the Target's DamageNegation and Health. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGAttributeSet, DamageNegation, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGAttributeSet, Posture, Target, false);
	}
};

static const ARPGChargeDamageStatics& DamageStatics()
{
	static ARPGChargeDamageStatics DStatics;
	return DStatics;
}

UARPGChargeAttackExecCalculation::UARPGChargeAttackExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageNegationDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().PostureDef);

	ChanceTag = FGameplayTag::RequestGameplayTag(FName("State.Chance"));
}

void UARPGChargeAttackExecCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float DamageNegation = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageNegationDef, EvaluationParameters,
	                                                           DamageNegation);
	DamageNegation = FMath::Max<float>(DamageNegation, 0.0f);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters,
	                                                           Damage);
	// Add SetByCaller damage if it exists
	Damage *= FMath::Max<float>(
		Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here

	float MitigatedDamage = (UnmitigatedDamage) * (1.f - DamageNegation);

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		// OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(UARPGAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive,
			                               MitigatedDamage));
	}

	// Posture Damage
	if (TargetAbilitySystemComponent->HasMatchingGameplayTag(ChanceTag))
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UARPGAttributeSet::GetPostureAttribute(), EGameplayModOp::Additive, -5000.f));
	}
	else if ((TargetActor->GetActorLocation() - SourceActor->GetActorLocation()).Dot(TargetActor->GetActorForwardVector()) > 0.5f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UARPGAttributeSet::GetPostureAttribute(), EGameplayModOp::Additive, -5000.f));
	}
	else
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(UARPGAttributeSet::GetPostureAttribute(), EGameplayModOp::Additive, -25.f));
	}

	// Broadcast damages to Target ASC
	UARPGAbilitySystemComponent* TargetASC = Cast<UARPGAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UARPGAbilitySystemComponent* SourceASC = Cast<UARPGAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}
}
