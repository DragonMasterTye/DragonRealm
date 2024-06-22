// Copyright Landon Morrison 2024


#include "DR_TargetDummy.h"

#include "DR_AttributeComponent.h"

// Sets default values
ADR_TargetDummy::ADR_TargetDummy()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;

	AttributeComponent = CreateDefaultSubobject<UDR_AttributeComponent>("AttributeComponent");
	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADR_TargetDummy::OnHealthChanged);

}

void ADR_TargetDummy::OnHealthChanged(AActor* InstigatorActor, UDR_AttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	if(ensureAlways(Mesh))
	{
		if(Delta < 0)
		{
			// Turn on MF_DR_HitFlash
			Mesh->SetScalarParameterValueOnMaterials("DR_TimeToHitFlash", GetWorld()->TimeSeconds);
		}
	}
}