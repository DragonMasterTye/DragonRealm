// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "DRBaseDataTypes.h"
#include "Engine/DataAsset.h"
#include "DRBaseCharacterDataAsset.generated.h"

UCLASS()
class DRAGONREALM_API UDRBaseCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FDRCharacterData CharacterData;
};
