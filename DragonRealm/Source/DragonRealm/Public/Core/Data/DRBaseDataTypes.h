// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "DRBaseDataTypes.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UBlendSpace;
class UAnimationAsset;

USTRUCT(BlueprintType)
struct FDRCharacterData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DR|AS")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DR|AS")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};

USTRUCT(BlueprintType)
struct FDRCharacterAnimationData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DR|Animation")
	UBlendSpace* GroundMovementBlendSpace = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DR|Animation")
	UAnimationAsset* GroundIdleAnimation = nullptr;
	
};