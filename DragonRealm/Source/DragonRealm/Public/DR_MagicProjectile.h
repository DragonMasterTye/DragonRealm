// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DR_MagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
UCLASS()
class DRAGONREALM_API ADR_MagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADR_MagicProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
