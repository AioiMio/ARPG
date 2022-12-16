#pragma once
#include "Engine/DataTable.h"
#include "FItemTableRow.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// Item name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	// Item description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	// Item icon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowedClasses = "Texture2D, PaperSprite"))
	FSoftObjectPath Icon;

	// Max stack count
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxCount = 99;
};
