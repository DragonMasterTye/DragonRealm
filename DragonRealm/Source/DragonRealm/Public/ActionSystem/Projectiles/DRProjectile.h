// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DRProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class DRAGONREALM_API ADRProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRProjectile();

	virtual void BeginPlay() override;

protected:
	// Unreal functions
	virtual void PostInitializeComponents() override;
	
	// Physical Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	// Imaginary Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Effects
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* SpawnVFX;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* ImpactSFX;

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();
	UFUNCTION(BlueprintNativeEvent)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
