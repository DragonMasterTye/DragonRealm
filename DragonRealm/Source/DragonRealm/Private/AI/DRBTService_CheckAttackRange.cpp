// Copyright Landon Morrison 2024


#include "AI/DRBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDRBTService_CheckAttackRange::UDRBTService_CheckAttackRange()
{
	AttackRange = 1000.f;
}

void UDRBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check Distance between AIPawn and TargetActor
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(ensureAlways(BlackboardComponent))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* AIC = OwnerComp.GetAIOwner();
			if(ensureAlways(AIC))
			{
				APawn* AIPawn = AIC->GetPawn();
				if(ensureAlways(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < AttackRange;
					bool bHasLOS = false;
					if(bWithinRange)
					{
						bHasLOS = AIC->LineOfSightTo(TargetActor);
					}

					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}

				
			}
		}
	}
}
