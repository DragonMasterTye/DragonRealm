// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/DRAction.h"
#include "DRAction_SpawnProjectile.generated.h"

class ADRProjectile;
class UParticleSystem;

UCLASS()
class DRAGONREALM_API UDRAction_SpawnProjectile : public UDRAction
{
	GENERATED_BODY()
	
protected:

	//  Properties
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	TSubclassOf<ADRProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	UParticleSystem* SpawnVFX;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	USoundBase* SpawnSFX;
	UPROPERTY(EditAnywhere, Category="DR|Projectile")
	FVector SpawnLocation;
	
};
