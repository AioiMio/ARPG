#pragma once

UENUM(BlueprintType)
enum class EARPGHitReactDirection : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Front UMETA(DisplayName = "Front"),
	Back UMETA(DisplayName = "Back"),
	
	EHRD_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EARPGAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"),
	
	EAII_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EAnimationType : uint8
{
	EAT_FullBody UMETA(DisplayName = "FullBody"),
	EAT_UpperBody UMETA(DisplayName = "UpperBody"),
	EAT_RightArm UMETA(DisplayName = "RightArm"),
	EAT_LeftArm UMETA(DisplayName = "LeftArm"),
	
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EEquipPostion : uint8
{
	RightHand UMETA(DisplayName = "RightHand"),
	LeftHand UMETA(DisplayName = "RightHand"),
	
	MAX UMETA(DisplayName = "DefaultMAX")
};
