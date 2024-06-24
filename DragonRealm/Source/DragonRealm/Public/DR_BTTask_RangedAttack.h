// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DR_BTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDR_BTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UDR_BTTask_RangedAttack();
	
	// Unreal Functions
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	// Properties
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;
};
