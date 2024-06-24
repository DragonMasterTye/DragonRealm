// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DR_BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDR_BTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

	UDR_BTService_CheckAttackRange();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange;
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;
	
	// Unreal Functions
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
