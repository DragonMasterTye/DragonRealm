// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DRAction.generated.h"

UCLASS(Blueprintable)
class DRAGONREALM_API UDRAction : public UObject
{
	GENERATED_BODY()

public:

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "DR|Assignables")
	FName ActionName;

	// Functions
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	void StartAction(AActor* Instigator); // Since this is a UObject, we don't already have an Instigator so this is a safe name
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	void StopAction(AActor* Instigator);
	
};
