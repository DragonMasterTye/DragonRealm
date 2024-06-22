// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DR_Projectile_Base.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class DRAGONREALM_API ADR_Projectile_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADR_Projectile_Base();

protected:
	// Unreal functions
	virtual void PostInitializeComponents() override;
	
	// Physical Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// Imaginary Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Effects
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
