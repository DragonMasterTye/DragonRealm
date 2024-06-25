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
void UDRAction::StartAction_Implementation(AActor* Instigator)
{
	if(CVarDebugAction.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s has STARTED."), *GetNameSafe(this));
	}

	UDRActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UDRAction::StopAction_Implementation(AActor* Instigator)
{
	if(CVarDebugAction.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s has STOPPED."), *GetNameSafe(this));
	}

	UDRActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
}

UDRActionComponent* UDRAction::GetOwningComponent() const
{
	return Cast<UDRActionComponent>(GetWorld());
}
