// Fill out your copyright notice in the Description page of Project Settings.


#include "World/DRExplosiveBarrel.h"

#include "ActionSystem/DRAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ADRExplosiveBarrel::ADRExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(MeshComponent);
	SphereComponent->SetSphereRadius(750.f);
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	ForceComponent = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComponent->SetupAttachment(MeshComponent);

	// Leaving this on would apply constant force, we only want a burst
	ForceComponent->SetAutoActivate(false);

	ForceComponent->Radius = 750.0f;
	
	ForceComponent->ImpulseStrength = 2000.0f;
	ForceComponent->bImpulseVelChange = true; // Ignores Mass so we don't have to do real world calculations

	ForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ADRExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADRExplosiveBarrel::OnActorOverlap);
}

void ADRExplosiveBarrel::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetInstigator() || this->GetVelocity().Length() > 1000.0f)
	{		
		Explode();
	}

	UE_LOG(LogTemp, Log, TEXT("OnStaticMeshBeginOverlap in Explosive Barrel"));

	UE_LOG(LogTemp, Log, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Overlap at Location: %s "), *GetActorLocation().ToString());
	DrawDebugString(GetWorld(), GetActorLocation(), CombinedString, nullptr, FColor::Green, 2.0f, true);
	
}

void ADRExplosiveBarrel::Explode_Implementation()
{
	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors);
	for (auto OverlappingActor : OverlappingActors)
	{
		UDRAttributeComponent* AttributeComponent = UDRAttributeComponent::GetAttributeComponent(OverlappingActor);
		
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(this, -90.f);
		}
	}
	
	ForceComponent->FireImpulse();
}
