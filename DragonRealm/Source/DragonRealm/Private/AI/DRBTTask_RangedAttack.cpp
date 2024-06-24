// Copyright Landon Morrison 2024


#include "AI/DRBTTask_RangedAttack.h"

#include "AIController.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UDRBTTask_RangedAttack::UDRBTTask_RangedAttack()
{
	MaxBulletSpread = 2.f;
}

EBTNodeResult::Type UDRBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if(ensureAlways(AIC))
	{
		ACharacter* AIPawn = Cast<ACharacter>(AIC->GetPawn());
		if(!AIPawn)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if(!TargetActor)
		{
			return EBTNodeResult::Failed;
		}

		if(!UDRAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AIPawn;

		AActor* NewProjectile = nullptr;
		if(ensureAlways(ProjectileClass))
		{
			NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		}
		
		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
