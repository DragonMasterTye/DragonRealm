// Copyright Landon Morrison 2024


#include "DR_AICharacter_Base.h"

#include "AIController.h"
#include "DR_AttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ADR_AICharacter_Base::ADR_AICharacter_Base()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<UDR_AttributeComponent>("AttributeComponent");
}

void ADR_AICharacter_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ADR_AICharacter_Base::OnPawnSeen);
}

bool ADR_AICharacter_Base::IsAlive()
{
	return AttributeComponent->IsAlive();
}

void ADR_AICharacter_Base::OnPawnSeen_Implementation(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}

