// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DRMonsterData.generated.h"

class UDRAction;
UCLASS()
class DRAGONREALM_API UDRMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Spawn Info")
	TSubclassOf<AActor> MonsterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Spawn Info")
	TArray<TSubclassOf<UDRAction>> Actions;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|UI")
	UTexture2D* Icon;

	// Unreal Functions
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}
};
