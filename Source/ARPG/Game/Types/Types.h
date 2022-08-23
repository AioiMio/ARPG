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