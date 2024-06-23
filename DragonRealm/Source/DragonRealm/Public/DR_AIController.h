// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DR_AIController.generated.h"

UCLASS()
class DRAGONREALM_API ADR_AIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	// Unreal Functions
	virtual void BeginPlay() override;

	
	
};
