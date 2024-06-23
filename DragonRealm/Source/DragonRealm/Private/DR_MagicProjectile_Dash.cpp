// Copyright Landon Morrison 2024


#include "DR_MagicProjectile_Dash.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ADR_MagicProjectile_Dash::ADR_MagicProjectile_Dash()
{
	ProjectileMovementComponent->InitialSpeed = 6000.f;
}

void ADR_MagicProjectile_Dash::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADR_MagicProjectile_Dash::Explode, DetonateDelay);
}

void ADR_MagicProjectile_Dash::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, GetActorLocation(), GetActorRotation());

	ParticleSystemComponent->DeactivateSystem();
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADR_MagicProjectile_Dash::TeleportInstigator, TeleportDelay);
}

void ADR_MagicProjectile_Dash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if(ensureAlways(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}
