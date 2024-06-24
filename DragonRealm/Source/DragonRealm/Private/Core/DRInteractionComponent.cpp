// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DRInteractionComponent.h"
#include "Core/DRGameplayInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("DR.DebugDrawInteraction"), false, TEXT("Enable Debug lines for InteractionComponent"), ECVF_Cheat);

// Sets default values for this component's properties
UDRInteractionComponent::UDRInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDRInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDRInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDRInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		if(bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, LineColor, false, 2.0f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<UDRGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(Owner);
			
				IDRGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				break;
			}
		}
	}
	
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}