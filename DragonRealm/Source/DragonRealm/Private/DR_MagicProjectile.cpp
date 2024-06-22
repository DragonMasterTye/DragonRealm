// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_MagicProjectile.h"

#include "DR_AttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADR_MagicProjectile::ADR_MagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADR_MagicProjectile::OnActorOverlap);
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;

	// TODO
	// Effects NOT WORKING
	// ImpactVFX = CreateDefaultSubobject<UParticleSystem>("ImpactVFX");
}

// Called when the game starts or when spawned
void ADR_MagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADR_MagicProjectile::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UDR_AttributeComponent* AttributeComp = Cast<UDR_AttributeComponent>(OtherActor->GetComponentByClass(UDR_AttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.f);
		}

		/*if(ensureAlways(ImpactVFX))
		{
			// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Impact, GetActorLocation());

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation());
		}*/
		
		Destroy();

	}
}

// Called every frame
void ADR_MagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

