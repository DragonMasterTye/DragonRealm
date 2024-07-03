// Copyright Landon Morrison 2024


#include "ActionSystem/Projectiles/DRProjectile.h"

#include "ActionSystem/DRActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

static TAutoConsoleVariable<bool> CVarDebugProjectile(TEXT("DR.DebugProjectile"), true, TEXT("Enable Debug messages, logs, and draws for Proejctiles"), ECVF_Cheat);

// Sets default values
ADRProjectile::ADRProjectile()
{
	bReplicates = true;
	InitialLifeSpan = 10.f;
	
	// Components
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetCollisionProfileName("Projectile");
	//SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADRProjectile::OnActorOverlap);
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
}

void ADRProjectile::BeginPlay()
{
	Super::BeginPlay();

	//UGameplayStatics::SpawnEmitterAtLocation(this, SpawnVFX, GetActorLocation(), GetActorRotation());
}

void ADRProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ADRProjectile::Explode_Implementation()
{
	if(ensure(!IsPendingKillPending()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

void ADRProjectile::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UDRActionComponent* ActionComponent = Cast<UDRActionComponent>(OtherActor->GetComponentByClass(UDRActionComponent::StaticClass()));
		if(ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			ProjectileMovementComponent->Velocity = -ProjectileMovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		Explode();
	}
}

