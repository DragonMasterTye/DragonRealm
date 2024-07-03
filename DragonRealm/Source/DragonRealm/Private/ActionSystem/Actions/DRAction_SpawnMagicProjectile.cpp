// Copyright Landon Morrison 2024


#include "ActionSystem/Actions/DRAction_SpawnMagicProjectile.h"

#include "ActionSystem/Projectiles/DRProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UDRAction_SpawnMagicProjectile::UDRAction_SpawnMagicProjectile()
{
	SpawnSocketName = "Magic_R_Socket";
	SpawnAnimDelay = 0.2f;
}

void UDRAction_SpawnMagicProjectile::SpawnDelay_TimeElapsed(ACharacter* InstigatorCharacter)
{
	if(ensure(ProjectileClass))
	{
		FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(SpawnSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		// Trace with sphere for simple, forgiving aim
		FCollisionShape Shape;
    	Shape.SetSphere(20.f);
    
    	// Ignore Player
    	FCollisionQueryParams QueryParams;
    	QueryParams.AddIgnoredActor(InstigatorCharacter);

		// Objects to trace for
    	FCollisionObjectQueryParams ObjectParams;
    	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
    	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
    	
    	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (InstigatorCharacter->GetControlRotation().Vector() * 500.f);
    	FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000.f);
    	
    	FHitResult Hit;
		/*if(CVarDebugProjectile.GetValueOnGameThread())
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 3.f);
		}*/
    	if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, Shape, QueryParams))
    	{
    		TraceEnd = Hit.ImpactPoint;		
    	}
    
    	FRotator ProjectedRotation = FRotationMatrix::MakeFromX(TraceEnd - SpawnLocation).Rotator();
	    FTransform SpawnTransform = FTransform(ProjectedRotation,SpawnLocation);
        
        GetWorld()->SpawnActor<ADRProjectile>(ProjectileClass, SpawnTransform, SpawnParams);
	}

	StopAction(InstigatorCharacter);
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

		if(Character->HasAuthority())
		{
			FTimerHandle TimerHandle_SpawnDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "SpawnDelay_TimeElapsed", Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnDelay, Delegate, SpawnAnimDelay, false);
		}
	}
}
