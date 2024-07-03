// Copyright Landon Morrison 2024


#include "ActionSystem/Projectiles/DRMagicProjectile_Dash.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ADRMagicProjectile_Dash::ADRMagicProjectile_Dash()
{
	ProjectileMovementComponent->InitialSpeed = 6000.f;
}

void ADRMagicProjectile_Dash::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADRMagicProjectile_Dash::Explode, DetonateDelay);
}

void ADRMagicProjectile_Dash::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, GetActorLocation(), GetActorRotation());

	ParticleSystemComponent->DeactivateSystem();
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADRMagicProjectile_Dash::TeleportInstigator, TeleportDelay);
}

void ADRMagicProjectile_Dash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if(ensureAlways(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	Destroy();
}
