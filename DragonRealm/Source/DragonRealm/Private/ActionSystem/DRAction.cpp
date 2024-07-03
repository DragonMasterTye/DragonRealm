// Copyright Landon Morrison 2024


#include "ActionSystem/DRAction.h"

#include "ActionSystem/DRActionComponent.h"
#include "DragonRealm/DragonRealm.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<bool> CVarDebugAction(TEXT("DR.DebugAction"), false, TEXT("Enable Debug messages, logs, and draws for actions"), ECVF_Cheat);

// Init
void UDRAction::Initialize(UDRActionComponent* NewActionComponent)
{
	OwningActionComponent = NewActionComponent;
}

// Unreal Functions
UWorld* UDRAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
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
	if(OwningActionComponent->GetOwner()->HasAuthority() && CVarDebugAction.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Log, TEXT("Action: %s has STARTED."), *GetNameSafe(this));
		DRLogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	}

	UDRActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if(GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void UDRAction::StopAction_Implementation(AActor* Instigator)
{
	// ensureAlways(bIsRunning); // Not Correct in Multiplayer
	
	if(OwningActionComponent->GetOwner()->HasAuthority() && CVarDebugAction.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Log, TEXT("Action: %s has STOPPED."), *GetNameSafe(this));
		DRLogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::Black);
	}

	UDRActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool UDRAction::IsRunning_Implementation() const
{
	return RepData.bIsRunning;
}

UDRActionComponent* UDRAction::GetOwningComponent() const
{
	return OwningActionComponent;
	
	//return Cast<UDRActionComponent>(GetOuter());
	// In Multiplayer, Unreal uses Actors as Outers, can't use Components
	// The Above is theoretically not true as of 5.0+ (haven't tested myself yet)
}

// Replication
void UDRAction::OnRep_RepData()
{
	// Utilizing the difference of Client vs Server to only trigger StartAction once even if we Predict it
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UDRAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDRAction, OwningActionComponent);
	DOREPLIFETIME(UDRAction, RepData);
	DOREPLIFETIME(UDRAction, TimeStarted);
}