// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG/Game/Abilities/ARPGGameplayAbility.h"
#include "ARPG/Game/Types/Types.h"
#include "Components/AGR_CombatManager.h"
#include "ARPGCombatManager.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Light UMETA(DisplayName = "Light"),
	Normal UMETA(DisplayName = "Normal"),
	Heavy UMETA(DisplayName = "Heavy"),
	Charge UMETA(DisplayName = "Charge"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EHitReactType : uint8
{
	None UMETA(DisplayName = "None"),
	KnockDown UMETA(DisplayName = "KnockDown"),
	KnockUp UMETA(DisplayName = "KnockUp"),
	KnockBack UMETA(DisplayName = "KnockBack"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECharacterCamp : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemy UMETA(DisplayName = "Enemy"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

class AARPGCharacter;
class UForceFeedbackEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBackstabTargetCharacterCountChanged, int32, OldTargetCharacterCount,
                                             int32, NewTargetCharacterCount);

/**
 * 
 */
UCLASS()
class ARPG_API UARPGCombatManager : public UAGR_CombatManager
{
	GENERATED_BODY()

public:
	UARPGCombatManager();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE TArray<AActor*> GetIgnoredActors() { return IgnoredActors; }

	FORCEINLINE void SetCurrentAttackType(EAttackType AttackType) { CurrentAttackType = AttackType; }
	FORCEINLINE EAttackType GetCurrentAttackType() const { return CurrentAttackType; }

	FORCEINLINE void SetCurrentHitReactType(EHitReactType HitReactType) { CurrentHitReactType = HitReactType; }
	FORCEINLINE EHitReactType GetCurrentHitReactType() const { return CurrentHitReactType; }

	FORCEINLINE void SetCurrentSkillMultiplier(float InMultiplier) { CurrentSkillMultiplier = InMultiplier; }
	FORCEINLINE float GetCurrentSkillMultiplier() const { return CurrentSkillMultiplier; }

	void SendHitEventToActor(AActor* Target, FHitResult Hit, EAttackType AttackType, EHitReactType HitReactType, float Multiplier = 1.f);

	void SendGameplayEventToOwner(FGameplayTag EventTag, FGameplayEventData Payload);
	UFUNCTION(Server, Reliable)
	void ServerSendGameplayEventToOwner(FGameplayTag EventTag, FGameplayEventData Payload);
	UFUNCTION(Client, Reliable)
	void ClientSendGameplayEventToOwner(FGameplayTag EventTag, FGameplayEventData Payload);
	
	bool TryVisceralAttack(bool bUseLockOnTarget = false);
	bool TryExecuteVisceralAttackToTarget(AARPGCharacter* Target);

	UFUNCTION(BlueprintCallable, Category = "Camp")
	FORCEINLINE ECharacterCamp GetCharacterCamp() const { return CharacterCamp; }

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AARPGCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camp")
	ECharacterCamp CharacterCamp;

	UPROPERTY()
	TArray<AActor*> HitActors;

	FGameplayTag HitEventTag;

	EAttackType CurrentAttackType = EAttackType::Normal;
	EHitReactType CurrentHitReactType = EHitReactType::None;

	float CurrentSkillMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Damage")
	TSubclassOf<UGameplayEffect> ChargeDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Passive")
	TSubclassOf<UGameplayEffect> KnockDownEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Passive")
	TSubclassOf<UGameplayEffect> KnockBackEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Passive")
	TSubclassOf<UGameplayEffect> KnockUpEffect;

	UFUNCTION()
	void OnAttackStart();

	UFUNCTION()
	void OnAttackEnd();

	UFUNCTION()
	void OnAttackHit(FHitResult Hit, UPrimitiveComponent* Mesh);

	UFUNCTION()
	void ApplyHitReact(EARPGHitReactDirection Direction);
};
