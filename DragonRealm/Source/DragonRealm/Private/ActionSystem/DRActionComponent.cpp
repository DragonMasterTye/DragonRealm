// Copyright Landon Morrison 2024


#include "ActionSystem/DRActionComponent.h"

#include "ActionSystem/DRAction.h"

// Unreal Functions
UDRActionComponent::UDRActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDRActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDRActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Functions ----------------------------------------------------------
void UDRActionComponent::AddAction(TSubclassOf<UDRAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UDRAction* NewAction = NewObject<UDRAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UDRActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UDRAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
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
		if(Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

