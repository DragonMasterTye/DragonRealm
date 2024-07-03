// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "DRProjectile.h"
#include "DRMagicProjectile_Dash.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API ADRMagicProjectile_Dash : public ADRProjectile
{
	GENERATED_BODY()

public:

	ADRMagicProjectile_Dash();
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay = 0.2f;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();
};
