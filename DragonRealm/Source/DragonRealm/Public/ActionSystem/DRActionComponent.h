// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DRActionComponent.generated.h"

class UDRAction;
class UDRActionComponent;
// Delegate for things like OnActionStarted and OnActionStopped
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UDRActionComponent*, OwningComponent, UDRAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Ctor
	UDRActionComponent();

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DR|Tags")
	FGameplayTagContainer ActiveGameplayTags;

	// Action Functions
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UDRAction> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	void RemoveAction(UDRAction* ActionToRemove);
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	
	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

	// Replication
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:
	
	// Properties
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UDRAction*> Actions; // Array of actions available to this ActionComponent
	UPROPERTY(EditAnywhere, Category = "DR|Actions")
	TArray<TSubclassOf<UDRAction>> DefaultActions; // Array of Actions available to this ActionComponent on GameStart

	// Unreal Functions
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Replication
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);
	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);
	
};
