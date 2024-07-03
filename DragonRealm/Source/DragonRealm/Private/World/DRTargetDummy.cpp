// Copyright Landon Morrison 2024


#include "World/DRTargetDummy.h"

#include "ActionSystem/DRAttributeComponent.h"

// Sets default values
ADRTargetDummy::ADRTargetDummy()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;

	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");

	TimeOfHitParamName = "DR_TimeOfHit";
}


void ADRTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADRTargetDummy::OnCurrentHealthChanged);
}

void ADRTargetDummy::OnCurrentHealthChanged_Implementation(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth,
                                             float Delta, float ActualDelta)
{
	if(ensureAlways(Mesh))
	{
		if(Delta < 0)
		{
			// Turn on MF_DR_HitFlash
			Mesh->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);
		}
	}
}
