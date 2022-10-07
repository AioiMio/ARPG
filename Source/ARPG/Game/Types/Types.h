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
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Sprint UMETA(DisplayName = "Sprint"),
	Block UMETA(DisplayName = "Block"),
	RightHeavy UMETA(DisplayName = "RightHeavy"),

	MAX UMETA(DisplayName = "DefaultMAX")
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

UENUM(BlueprintType)
enum class EElevatorPosition : uint8
{
	None UMETA(DisplayName = "None"),
	Up UMETA(DisplayName = "Up"),
	Down UMETA(DisplayName = "Down"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Mythical UMETA(DisplayName = "Mythical"),
	Legendary UMETA(DisplayName = "Legendary"),

	MAX UMETA(DisplayName = "DefaultMAX")
};
