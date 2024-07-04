// Copyright Landon Morrison 2024


#include "World/DRPickupActor.h"

#include "Components/SphereComponent.h"

// Sets default values
ADRPickupActor::ADRPickupActor()
{
	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	SphereComponent->SetCollisionProfileName("Pickup");
	RootComponent = SphereComponent;
}

// Functions ---------------------------------
void ADRPickupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	IDRGameplayInterface::Interact_Implementation(InstigatorPawn);

	HideAndCooldownPickup();
}

void ADRPickupActor::SetPickupState_Implementation(bool bNewState)
{
	SetActorEnableCollision(bNewState);
	
	RootComponent->SetVisibility(bNewState, true);
}

void ADRPickupActor::ShowPickup_Implementation()
{
	SetPickupState(true);
}

void ADRPickupActor::HideAndCooldownPickup_Implementation()
{
	SetPickupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ADRPickupActor::ShowPickup, RespawnTime);
}