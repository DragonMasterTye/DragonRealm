// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "DR_Projectile_Base.h"
#include "DR_MagicProjectile_Dash.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API ADR_MagicProjectile_Dash : public ADR_Projectile_Base
{
	GENERATED_BODY()

public:

	ADR_MagicProjectile_Dash();
	
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
