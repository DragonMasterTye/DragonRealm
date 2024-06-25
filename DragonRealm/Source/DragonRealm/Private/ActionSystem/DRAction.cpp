// Copyright Landon Morrison 2024


#include "ActionSystem/DRAction.h"

static TAutoConsoleVariable<bool> CVarDebugAction(TEXT("DR.DebugAction"), true, TEXT("Enable Debug messages, logs, and draws for actions"), ECVF_Cheat);

void UDRAction::StartAction_Implementation(AActor* Instigator)
{
	if(CVarDebugAction.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s has STARTED."), *GetNameSafe(this));
	}
}

void UDRAction::StopAction_Implementation(AActor* Instigator)
{
	if(CVarDebugAction.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s has STOPPED."), *GetNameSafe(this));
	}
}

UWorld* UDRAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}
