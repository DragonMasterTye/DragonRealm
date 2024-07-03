// Copyright Landon Morrison 2024


#include "Core/DRLockOnComponent.h"

#include "DragonRealm/DragonRealm.h"
#include "Player/DRPlayerCharacter.h"

// Ctor
UDRLockOnComponent::UDRLockOnComponent()
{
	Period = 0.03f;
}

// Unreal Functions
void UDRLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// Start "tick" timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SearchForLock, this, &UDRLockOnComponent::SearchForLock, Period, true);
}

// Functions ---------------------------------
void UDRLockOnComponent::SearchForLock()
{
	ADRPlayerCharacter* Owner = Cast<ADRPlayerCharacter>(GetOwner());
	if(!Owner)
	{
		return;
	}

	DRLogOnScreen(this, FString::Printf(TEXT("Time: %f"), GetWorld()->GetTimeSeconds()), FColor::White, 0.0f);
}
