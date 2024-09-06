// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DRBaseInteractionComponent.h"
#include "Core/DRGameplayInterface.h"
#include "World/DRWorldUserWidget.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("DR.DebugDrawInteraction"), false, TEXT("Enable Debug lines for InteractionComponent"), ECVF_Cheat);

// Ctor
UDRBaseInteractionComponent::UDRBaseInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 500.f;
	TraceChannel = ECC_WorldDynamic;
}

// Unreal Functions
void UDRBaseInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UDRBaseInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Functions
void UDRBaseInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UDRBaseInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceChannel);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	FocusedActor = nullptr;
	
	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		if(bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, LineColor, false, 0.f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<UDRGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if(FocusedActor)
	{
		if(DefaultInteractionWidget == nullptr && ensure(DefaultInteractionWidgetClass))
		{
			DefaultInteractionWidget = CreateWidget<UDRWorldUserWidget>(GetWorld(), DefaultInteractionWidgetClass);
		}

		if(DefaultInteractionWidget)
		{
			DefaultInteractionWidget->AttachedActor = FocusedActor;

			if(!DefaultInteractionWidget->IsInViewport())
			{
				DefaultInteractionWidget->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultInteractionWidget)
		{
			DefaultInteractionWidget->RemoveFromParent();
		}
	}
	
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 0.f, 0, 2.0f);
	}
}

void UDRBaseInteractionComponent::ServerInteract_Implementation(AActor* InFocusedActor)
{
	if(InFocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No FocusedActor to Interact with");
		return;
	}
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
			
	IDRGameplayInterface::Execute_Interact(InFocusedActor, OwnerPawn);
}
