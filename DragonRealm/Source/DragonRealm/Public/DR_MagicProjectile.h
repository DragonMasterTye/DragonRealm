// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DR_MagicProjectile.generated.h"

//class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;
UCLASS()
class DRAGONREALM_API ADR_MagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADR_MagicProjectile();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// UNiagaraSystem* NS_Impact;

	// TODO
	// Effects
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effects")
	// UParticleSystem* ImpactVFX;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
