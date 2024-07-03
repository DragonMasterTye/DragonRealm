// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/DRAction.h"
#include "DRAction_SpawnMagicProjectile.generated.h"


class ADRProjectile;
UCLASS()
class DRAGONREALM_API UDRAction_SpawnMagicProjectile : public UDRAction
{
	GENERATED_BODY()

public:

	UDRAction_SpawnMagicProjectile();

protected:

	// Properties
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	UAnimMontage* SpawnAnim;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	float SpawnAnimDelay;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	TSubclassOf<ADRProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	UParticleSystem* SpawnVFX;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	USoundBase* SpawnSFX;
	UPROPERTY(VisibleAnywhere, Category="DR|Projectile")
	FName SpawnSocketName;
	
	// Functions
	UFUNCTION()
	void SpawnDelay_TimeElapsed(ACharacter* InstigatorCharacter);

	virtual void StartAction_Implementation(AActor* Instigator) override;
};
