// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DRBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

	UDRBTService_CheckAttackRange();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange;
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;
	
	// Unreal Functions
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
