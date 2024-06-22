// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_ExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ADR_ExplosiveBarrel::ADR_ExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	ForceComponent = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComponent->SetupAttachment(MeshComponent);

	// Leaving this on would apply constant force, we only want a burst
	ForceComponent->SetAutoActivate(false);

	ForceComponent->Radius = 750.0f;
	
	ForceComponent->ImpulseStrength = 2000.0f;
	ForceComponent->bImpulseVelChange = true; // Ignores Mass so we don't have to do real world calculations

	ForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ADR_ExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADR_ExplosiveBarrel::OnActorOverlap);
}

void ADR_ExplosiveBarrel::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetInstigator() || this->GetVelocity().Length() > 1000.0f)
	{
		Explode();
	}

	UE_LOG(LogTemp, Log, TEXT("OnHit in Explosive Barrel"));

	UE_LOG(LogTemp, Log, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at Location: %s"), *GetActorLocation().ToString());
	DrawDebugString(GetWorld(), GetActorLocation(), CombinedString, nullptr, FColor::Green, 2.0f, true);
}

void ADR_ExplosiveBarrel::Explode_Implementation()
{
	ForceComponent->FireImpulse();
}
