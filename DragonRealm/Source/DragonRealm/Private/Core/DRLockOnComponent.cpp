// Copyright Landon Morrison 2024


#include "Core/DRLockOnComponent.h"

#include "DragonRealm/DragonRealm.h"
#include "Player/DRPlayerCharacter.h"

// Ctor
UDRLockOnComponent::UDRLockOnComponent()
{
	TicksPerSecond = 30.f;
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
	
	//DRLogOnScreen(this, FString::Printf(TEXT("Time: %f"), GetWorld()->GetTimeSeconds()), FColor::White, 0.0f);

	// Sweep for Targetables

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
