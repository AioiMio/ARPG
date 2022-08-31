// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ARPG/Game/Types/Types.h"
#include "GameFramework/Character.h"
#include "ARPGCharacter.generated.h"

class UARPGEquipmentComponent;
class UARPGCombatManager;
class UARPGAbilitySystemComponent;
class UARPGAttributeSet;
class UARPGGameplayAbility;
class UGameplayEffect;
class UARPGHealthBarWidget;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHitReactDelegate, EARPGHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AARPGCharacter*, Character);


UCLASS()
class ARPG_API AARPGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AARPGCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void PostInitializeComponents() override;

	// Set the Hit React direction in the Animation Blueprint
	UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterHitReactDelegate ShowHitReact;

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterDiedDelegate OnCharacterDied;

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const { return AbilitySystemComponent.Get(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UARPGCombatManager* GetCombatManager() const { return CombatManager; }

	FORCEINLINE UARPGHealthBarWidget* GetHealthBar() const { return HealthBar; }

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool IsAlive() const;

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual int32 GetAbilityLevel(EARPGAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable)
	EARPGHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual void PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual bool PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser);

	/**
	* Input Actions
	**/
	
	UFUNCTION(BlueprintCallable, Category = "InputActions")
	virtual void JumpAction();

	UFUNCTION(BlueprintCallable, Category = "InputActions")
	virtual void RightHandAttackAction();


	/**
	* Getters for attributes from GDAttributeSetBase
	**/
	
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxStamina() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMoveSpeedBaseValue() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void FinishDying();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Instead of TWeakObjectPtrs, you could just have UPROPERTY() hard references or no references at all and just call
	// GetAbilitySystem() and make a GetAttributeSetBase() that can read from the PlayerState or from child classes.
	// Just make sure you test if the pointer is valid before using.
	// I opted for TWeakObjectPtrs because I didn't want a shared hard reference here and I didn't want an extra function call of getting
	// the ASC/AttributeSet from the PlayerState or child classes every time I referenced them in this base class.

	TWeakObjectPtr<UARPGAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UARPGAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UARPGEquipmentComponent* EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UARPGCombatManager* CombatManager;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UARPGHealthBarWidget> HealthBarClass;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	UWidgetComponent* HealthBarComponent;

	UPROPERTY()
	UARPGHealthBarWidget* HealthBar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	UWidgetComponent* LockOnPointComponent;

	UPROPERTY(EditAnywhere, Category = "Character")
	FText CharacterName;

	FGameplayTag HitEventTag;
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;
	FGameplayTag FallingTag;
	FGameplayTag JumpTag;
	FGameplayTag RightHandAttackTag;

	FTimerHandle TimerHandle_LandDelay;
	FTimerHandle TimerHandle_FallingTagRemoveDelay;

	UFUNCTION()
	void LandElapsed();

	UFUNCTION()
	void RemoveFallingTagElapsed();

	// Death Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UARPGGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	// Creates and initializes the health bar.
	// Safe to call many times because it checks to make sure it only executes once.
	UFUNCTION()
	void InitializeHealthBar();


	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/

	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);
	virtual void SetStamina(float Stamina);
};
