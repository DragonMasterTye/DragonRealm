// Copyright Landon Morrison 2024


#include "ActionSystem/DRAction.h"

#include "ActionSystem/DRActionComponent.h"

static TAutoConsoleVariable<bool> CVarDebugAction(TEXT("DR.DebugAction"), true, TEXT("Enable Debug messages, logs, and draws for actions"), ECVF_Cheat);

// Unreal Functions
UWorld* UDRAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

// Functions --------------------------------------------------
bool UDRAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	
	UDRActionComponent* ActionComponent = GetOwningComponent();
	if(ActionComponent->ActiveGameplayTags.HasAny(BlockedByTags))
	{
		return false;
	}
	return true;
}

void UDRAction::StartAction_Implementation(AActor* Instigator)
{
	if(CVarDebugAction.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s has STARTED."), *GetNameSafe(this));
	}

	UDRActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UDRAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning);
	
	if(CVarDebugAction.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s has STOPPED."), *GetNameSafe(this));
	}

	UDRActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool UDRAction::IsRunning_Implementation() const
{
	return bIsRunning;
}

UDRActionComponent* UDRAction::GetOwningComponent() const
{
	return Cast<UDRActionComponent>(GetOuter());
}
