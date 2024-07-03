// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "DRProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class DRAGONREALM_API ADRProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Ctor
	ADRProjectile();

	// Unreal Functions
	virtual void BeginPlay() override;

protected:
	// Unreal functions
	virtual void PostInitializeComponents() override;
	
	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	// Imaginary(Actor) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "DR|Projectile")
	FGameplayTag ParryTag;
	
	// Effects
	UPROPERTY(EditDefaultsOnly, Category = "DR|Effects")
	UParticleSystem* SpawnVFX;
	UPROPERTY(EditDefaultsOnly, Category = "DR|Effects")
	UParticleSystem* ImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category = "DR|Effects")
	USoundBase* ImpactSFX;

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();
	UFUNCTION(BlueprintNativeEvent)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
