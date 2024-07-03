// Copyright Landon Morrison 2024


#include "ActionSystem/DRActionComponent.h"

#include "ActionSystem/DRAction.h"
#include "DragonRealm/DragonRealm.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// CVAR
static TAutoConsoleVariable<bool> CVarDebugTags(TEXT("DR.DebugTags"), false, TEXT("Enable Debug messages and logs for Tags"), ECVF_Cheat);

// STAT
DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_DRStartActionByName, STATGROUP_DRAGONREALM);

// Ctor
UDRActionComponent::UDRActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

// Unreal Functions
void UDRActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority() && !DefaultActions.IsEmpty())
	{
		for(TSubclassOf<UDRAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UDRActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CVarDebugTags.GetValueOnGameThread())
	{
		//FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);

		// Draw all Actions
		for(UDRAction* Action : Actions)
		{
			FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
			FString ActionMsg =  FString::Printf(TEXT("[%s] Action: %s"),
				*GetNameSafe(GetOwner()),
				*GetNameSafe(Action));
			DRLogOnScreen(this, ActionMsg, TextColor, 0.0f);
		}
	}
}

// Functions ----------------------------------------------------------
void UDRActionComponent::AddAction(AActor* Instigator, TSubclassOf<UDRAction> ActionClass)
{
	if(!ActionClass)
	{
		return;
	}

	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UDRAction* NewAction = NewObject<UDRAction>(GetOwner(), ActionClass);
	if(NewAction)
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && NewAction->CanStart(Instigator))
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
	SCOPE_CYCLE_COUNTER(STAT_DRStartActionByName);
	
	for(UDRAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				if(GetOwner()->HasAuthority() && CVarDebugTags.GetValueOnGameThread())
				{
					FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				}
				continue;
			}

			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction: %s"), *GetNameSafe(Action));
			
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
			if(!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}
			
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

// Replication
void UDRActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UDRActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool UDRActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(UDRAction* Action : Actions)
	{
		if(Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UDRActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDRActionComponent, Actions);
}