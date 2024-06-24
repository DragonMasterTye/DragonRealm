// Copyright Landon Morrison 2024


#include "DR_AICharacter_Base.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DR_AttributeComponent.h"
#include "DR_WorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ADR_AICharacter_Base::ADR_AICharacter_Base()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<UDR_AttributeComponent>("AttributeComponent");

	TimeOfHitParamName = "DR_TimeOfHit";
	
}

void ADR_AICharacter_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ADR_AICharacter_Base::OnPawnSeen);
	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADR_AICharacter_Base::OnCurrentHealthChanged);
}

void ADR_AICharacter_Base::OnCurrentHealthChanged(AActor* InstigatorActor, UDR_AttributeComponent* OwningComponent,
	float NewHealth, float Delta)
{
	if(Delta < 0.f)
	{
		// Turn on MF_DR_HitFlash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UDR_WorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);

			// Remove Health Bar
			ActiveHealthBar->RemoveFromParent();
			
			SetLifeSpan(10.0f);
		}
	}
}

void ADR_AICharacter_Base::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

bool ADR_AICharacter_Base::IsAlive()
{
	return AttributeComponent->IsAlive();
}

void ADR_AICharacter_Base::OnPawnSeen_Implementation(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

