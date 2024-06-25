// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRActionComponent.generated.h"


class UDRAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Ctor
	UDRActionComponent();

	// Functions
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	void AddAction(TSubclassOf<UDRAction> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

protected:
	
	// Properties
	UPROPERTY()
	TArray<UDRAction*> Actions;
	UPROPERTY(EditAnywhere, Category = "DR|Actions")
	TArray<TSubclassOf<UDRAction>> DefaultActions;

	// Unreal Functions
	virtual void BeginPlay() override;
};
