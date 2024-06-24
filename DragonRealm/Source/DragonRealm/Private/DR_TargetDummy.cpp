// Copyright Landon Morrison 2024


#include "DR_TargetDummy.h"

#include "DR_AttributeComponent.h"

// Sets default values
ADR_TargetDummy::ADR_TargetDummy()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;

	AttributeComponent = CreateDefaultSubobject<UDR_AttributeComponent>("AttributeComponent");

	TimeOfHitParamName = "DR_TimeOfHit";
}


void ADR_TargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADR_TargetDummy::OnCurrentHealthChanged);
}

void ADR_TargetDummy::OnCurrentHealthChanged_Implementation(AActor* InstigatorActor, UDR_AttributeComponent* OwningComponent, float NewHealth,
                                             float Delta)
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
