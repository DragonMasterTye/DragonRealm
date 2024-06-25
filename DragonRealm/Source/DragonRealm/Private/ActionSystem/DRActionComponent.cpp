// Copyright Landon Morrison 2024


#include "ActionSystem/DRActionComponent.h"

#include "ActionSystem/DRAction.h"

static TAutoConsoleVariable<bool> CVarDebugTags(TEXT("DR.DebugTags"), true, TEXT("Enable Debug messages and logs for Tags"), ECVF_Cheat);

// Ctor
UDRActionComponent::UDRActionComponent()
{
	
}

// Unreal Functions
void UDRActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<UDRAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
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

