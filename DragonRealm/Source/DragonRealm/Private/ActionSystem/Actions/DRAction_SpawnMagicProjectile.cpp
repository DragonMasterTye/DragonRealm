// Copyright Landon Morrison 2024


#include "ActionSystem/Actions/DRAction_SpawnMagicProjectile.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UDRAction_SpawnMagicProjectile::UDRAction_SpawnMagicProjectile()
{
	SpawnSocketName = "Magic_R_Socket";
	SpawnAnimDelay = 0.2f;
}

void UDRAction_SpawnMagicProjectile::SpawnDelay_TimeElapsed(ACharacter* InstigatorCharacter)
{
	
}

void UDRAction_SpawnMagicProjectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(Character)
	{
		Character->PlayAnimMontage(SpawnAnim);
		UGameplayStatics::SpawnEmitterAttached(SpawnVFX, Character->GetMesh(), SpawnSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		UGameplayStatics::SpawnSoundAttached(SpawnSFX, Character->GetMesh(), SpawnSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_SpawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "SpawnDelay_TimeElapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnDelay, Delegate, SpawnAnimDelay, false);
	}
}
