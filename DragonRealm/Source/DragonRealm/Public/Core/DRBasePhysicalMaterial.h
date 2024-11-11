// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DRBasePhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRBasePhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DR|Material")
	USoundBase* FootstepSound = nullptr;
	
};
