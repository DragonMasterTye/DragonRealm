// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DRBaseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;
	
};
