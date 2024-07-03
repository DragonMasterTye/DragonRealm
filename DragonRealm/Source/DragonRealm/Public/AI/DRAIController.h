// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DRAIController.generated.h"

UCLASS()
class DRAGONREALM_API ADRAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	// Unreal Functions
	virtual void BeginPlay() override;

	
	
};
