// Copyright Landon Morrison 2024


#include "DR_PickupActor_Base.h"

#include "Components/SphereComponent.h"

// Sets default values
ADR_PickupActor_Base::ADR_PickupActor_Base()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	SphereComponent->SetCollisionProfileName("Pickup");
	RootComponent = SphereComponent;
}

void ADR_PickupActor_Base::Interact_Implementation(APawn* InstigatorPawn)
{
	IDR_GameplayInterface::Interact_Implementation(InstigatorPawn);

	HideAndCooldownPickup();
}

void ADR_PickupActor_Base::SetPickupState_Implementation(bool bNewState)
{
	SetActorEnableCollision(bNewState);
	
	RootComponent->SetVisibility(bNewState, true);
}

void ADR_PickupActor_Base::ShowPickup_Implementation()
{
	SetPickupState(true);
}

void ADR_PickupActor_Base::HideAndCooldownPickup_Implementation()
{
	SetPickupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ADR_PickupActor_Base::ShowPickup, RespawnTime);
}