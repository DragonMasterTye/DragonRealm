// Copyright Landon Morrison 2024


#include "Core/DRBaseLockOnComponent.h"

#include "KismetTraceUtils.h"
#include "Camera/CameraComponent.h"
#include "Core/DRGameplayInterface.h"
#include "DragonRealm/DragonRealm.h"
#include "Player/DRPlayerCharacter.h"

// Ctor
UDRBaseLockOnComponent::UDRBaseLockOnComponent()
{
	TicksPerSecond = 30.f;
	ConeTraceDistance = 1000.f;
	ConeTraceSteps = 10;
	ConeTraceStartRadius = 100.f;
	ConeTraceEndRadius = 1200.f;
	ConeTraceChannel = ECC_GameTraceChannel2;
}

// Unreal Functions
void UDRBaseLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
								// FPS * Seconds
	float Rate = TicksPerSecond / (30.f * 60.f);
	
	Owner = Cast<ADRPlayerCharacter>(GetOwner());
	if(Owner) // Only run our "tick" function if we have a DRPlayerCharacter as owner
	{
		// Start "tick" timer
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle_SearchForTargetables, this, &UDRBaseLockOnComponent::SearchForTargetables, Rate, true);
	}
}	

// Functions ---------------------------------
void UDRBaseLockOnComponent::SearchForTargetables()
{
	// There is potential for improving efficiency by using the method from PawnSensingComponent
	// to access all *Relevant* Pawns and then see if they are within the bounds, @todo try this
	Owner = Cast<ADRPlayerCharacter>(GetOwner());
	// Cone Trace using multiple Sphere Traces of expanding size
	UCameraComponent* Camera = Owner->GetCamera();
	ConeTraceOffset = Camera->GetComponentLocation(); // Starting Position
	for(float i = 1; i < (ConeTraceSteps + 1); i++)
	{
		// Array of Hit Results from this trace
		TArray<FHitResult> Hits;

		// Calculate End of current sphere trace
		float xyz = ConeTraceDistance * (1.0f / ConeTraceSteps);\
		FVector Temp(xyz);		
		FVector ToBeAdded = Camera->GetForwardVector() * Temp;
		FVector End = ConeTraceOffset + ToBeAdded;

		// Calculate Radius of current sphere trace
		UE::Math::TVector2<float> InputRange(0.f, 1.f);
		UE::Math::TVector2<float> OutputRange(ConeTraceStartRadius, ConeTraceEndRadius);
		float Value = i * (1.0f / ConeTraceSteps);
		float Radius = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, Value);
		
		// Collision Params
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);
		
		bool const bBlockingHit = GetWorld()->SweepMultiByChannel(Hits, ConeTraceOffset, End, FQuat::Identity, ConeTraceChannel, FCollisionShape::MakeSphere(Radius), QueryParams);

		// Debugging
		//FColor TraceColor = bBlockingHit ? FColor::Green : FColor::Red;
		DrawDebugSphereTraceMulti(GetWorld(), ConeTraceOffset, End, Radius, EDrawDebugTrace::ForDuration, bBlockingHit, Hits, FColor::Red, FColor::Green, 5.f);
		/*for (FHitResult Hit : Hits)
		{
			// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10, 16, LineColor, false, 5.f);
		
			AActor* HitActor = Hit.GetActor();
			if(HitActor)
			{
				if(HitActor->Implements<UDRGameplayInterface>())
				{
					//FocusedActor = HitActor;
					break;
				}
			}
		}*/

		ConeTraceOffset = End;

		UE_LOG(LogTemp, Log, TEXT("Loop: "));
	}

	
	
	// DRLogOnScreen(this, FString::Printf(TEXT("Time: %f"), GetWorld()->GetTimeSeconds()), FColor::White, 0.0f);

	// Sweep for Targetables
	
	
	//if(!TargetedActor)
	//{
		/*TArray<FHitResult> Hits;
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
		}*/
	//}
	


	// Highlight Targetable
}

void UDRBaseLockOnComponent::LockOn()
{
	// Set Camera to follow Target (with a little wiggle room)
}

void UDRBaseLockOnComponent::LockOff()
{
	// Free Camera

	// Reset Target
}
