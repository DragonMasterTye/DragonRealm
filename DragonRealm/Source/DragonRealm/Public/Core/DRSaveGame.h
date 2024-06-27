// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DRSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 EXP;
	
};
