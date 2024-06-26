// Copyright Landon Morrison 2024


#include "ActionSystem/DRActionComponent.h"

#include "ActionSystem/DRAction.h"

static TAutoConsoleVariable<bool> CVarDebugTags(TEXT("DR.DebugTags"), true, TEXT("Enable Debug messages and logs for Tags"), ECVF_Cheat);

// Ctor
UDRActionComponent::UDRActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Unreal Functions
void UDRActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<UDRAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UDRActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CVarDebugTags.GetValueOnGameThread())
	{
		FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
	}
}

// Functions ----------------------------------------------------------
void UDRActionComponent::AddAction(AActor* Instigator, TSubclassOf<UDRAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UDRAction* NewAction = NewObject<UDRAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UDRActionComponent::RemoveAction(UDRAction* ActionToRemove)
{
	if(!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
}

bool UDRActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UDRAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				if(CVarDebugTags.GetValueOnGameThread())
				{
					FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				}
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UDRActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(UDRAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

