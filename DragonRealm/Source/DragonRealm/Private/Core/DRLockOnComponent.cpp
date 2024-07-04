// Copyright Landon Morrison 2024


#include "Core/DRLockOnComponent.h"

#include "Camera/CameraComponent.h"
#include "Core/DRGameplayInterface.h"
#include "DragonRealm/DragonRealm.h"
#include "Player/DRPlayerCharacter.h"

// Ctor
UDRLockOnComponent::UDRLockOnComponent()
{
	TicksPerSecond = 30.f;
	TraceDistance = 5000.f;
	TraceRadius = 200.f;
	TraceChannel = ECC_Visibility;
}

// Unreal Functions
void UDRLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

								// FPS * Seconds
	float Rate = TicksPerSecond / (30.f * 60.f);

	// Start "tick" timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SearchForLock, this, &UDRLockOnComponent::SearchForLock, Rate, true);
}

// Functions ---------------------------------
void UDRLockOnComponent::SearchForLock()
{
	ADRPlayerCharacter* Owner = Cast<ADRPlayerCharacter>(GetOwner());
	if(!Owner)
	{
		return;
	}
	
	// DRLogOnScreen(this, FString::Printf(TEXT("Time: %f"), GetWorld()->GetTimeSeconds()), FColor::White, 0.0f);

	// Sweep for Targetables
	//if(!TargetedActor)
	//{
		TArray<FHitResult> Hits;
		FVector Start = Owner->GetCamera()->GetComponentLocation();
		FRotator Rotation = Owner->GetCamera()->GetComponentRotation();
		FVector End = Start + (Rotation.Vector() * TraceDistance);
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(TraceChannel);
		FCollisionShape Shape;
		Shape.SetSphere(TraceRadius);
	
		bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red; // Debug Trace
		
		if(bBlockingHit)
		{
			for (FHitResult Hit : Hits)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, LineColor, false, 0.f); // Debug Trace
				
				AActor* HitActor = Hit.GetActor();
				if(HitActor && HitActor->GetClass()->ImplementsInterface(UDRGameplayInterface::StaticClass()))
				{
					if(IDRGameplayInterface::Execute_IsTargetable(HitActor))
					{
						TargetedActor = HitActor;
						DRLogOnScreen(this, FString::Printf(TEXT("HitActor: %s"), *GetNameSafe(HitActor)));
						break;
					}
				}
			}
		}
	//}
	


	// Highlight Targetable
}

void UDRLockOnComponent::LockOn()
{
	// Set Camera to follow Target (with a little wiggle room)
}

void UDRLockOnComponent::LockOff()
{
	// Free Camera

	// Reset Target
}
