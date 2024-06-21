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
	
public:	
	// Sets default values for this actor's properties
	ADR_ExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* ForceComponent;
	
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent)
	void Explode();
	
};
