// Copyright Landon Morrison 2024


#include "World/DRTargetDummy.h"

#include "ActionSystem/DRAttributeComponent.h"

ADRTargetDummy::ADRTargetDummy()
{
	TimeOfHitParamName = "DR_TimeOfHit";
}

void ADRTargetDummy::OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth,
	float DesiredDelta, float ActualDelta)
{
	Super::OnHealthChanged(InstigatorActor, OwningComponent, NewHealth, DesiredDelta, ActualDelta);

	if(ActualDelta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);

		if(NewHealth <= 0.0f)
		{
			AttributeComponent->FullHeal(InstigatorActor);
		}
	}
}
