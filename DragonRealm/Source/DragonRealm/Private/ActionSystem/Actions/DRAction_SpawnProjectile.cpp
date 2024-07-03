// Copyright Landon Morrison 2024


#include "ActionSystem/Actions/DRAction_SpawnProjectile.h"

UDRAction_SpawnProjectile::UDRAction_SpawnProjectile()
{
	SpawnSocketName = "MuzzleSocket";
}

void UDRAction_SpawnProjectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
}
