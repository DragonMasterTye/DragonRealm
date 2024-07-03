// Copyright Landon Morrison 2024


#include "ActionSystem/DRActionEffect.h"

#include "ActionSystem/DRActionComponent.h"
#include "GameFramework/GameStateBase.h"


UDRActionEffect::UDRActionEffect()
{
	bAutoStart = true;
}

// Functions
void UDRActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DurationHandle, Delegate, Duration, false);
	}

	if(Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PeriodHandle, Delegate, Period, true);
	}
}

void UDRActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DurationHandle);

	UDRActionComponent* ActionComponent = GetOwningComponent();
	if(ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
}

float UDRActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState();
	if(GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void UDRActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
	
}
