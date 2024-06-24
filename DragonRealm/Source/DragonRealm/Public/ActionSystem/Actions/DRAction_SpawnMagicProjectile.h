// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/Actions/DRAction_SpawnProjectile.h"
#include "DRAction_SpawnMagicProjectile.generated.h"


UCLASS()
class DRAGONREALM_API UDRAction_SpawnMagicProjectile : public UDRAction_SpawnProjectile
{
	GENERATED_BODY()

protected:

	// Properties
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	UAnimMontage* SpawnAnim;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	float SpawnAnimDelay;
};
