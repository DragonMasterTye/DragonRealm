// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DR_ExplosiveBarrel.generated.h"

class URadialForceComponent;
UCLASS()
class DRAGONREALM_API ADR_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForceComponent;
	
public:	
	// Sets default values for this actor's properties
	ADR_ExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
