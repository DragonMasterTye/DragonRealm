// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "DRBaseDataTypes.h"
#include "Engine/DataAsset.h"
#include "DRBaseCharacterAnimDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRBaseCharacterAnimDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FDRCharacterAnimationData CharacterAnimationData;
	
};
