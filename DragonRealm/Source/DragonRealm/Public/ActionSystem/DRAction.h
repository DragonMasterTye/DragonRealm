// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "DRAction.generated.h"

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	AActor* Instigator;
};

class UDRActionComponent;
class UWorld;
UCLASS(Blueprintable)
class DRAGONREALM_API UDRAction : public UObject
{
	GENERATED_BODY()

public:

	// Init
	void Initialize(UDRActionComponent* NewActionComponent);

	// Unreal Functions
	virtual UWorld* GetWorld() const override;

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "DR|Assignables")
	FName ActionName;
	UPROPERTY(EditDefaultsOnly, Category = "DR|Assignables")
	bool bAutoStart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Assignables")
	UTexture2D* Icon;
	//TSoftObjectPtr<UTexture2D> Icon; // Could be set as a soft reference but is rather small to matter

	// Functions
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	bool CanStart(AActor* Instigator); // Since this is a UObject, we don't already have an Instigator so this is a safe name
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	void StartAction(AActor* Instigator); 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DR|Actions")
	void StopAction(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	bool IsRunning() const;

	// Replication
	virtual bool IsSupportedForNetworking() const override { return true; }
	
protected:

	// Properties
	UPROPERTY(Replicated)
	UDRActionComponent* OwningActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DR|Tags")
	FGameplayTagContainer GrantsTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DR|Tags")
	FGameplayTagContainer BlockedByTags;
	UPROPERTY(Replicated)
	float TimeStarted;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	UDRActionComponent* GetOwningComponent() const;

	// Replication
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData; // OnRep function will ONLY trigger if this value CHANGES (must keep track of state on both Client and Server)
	UFUNCTION()
	void OnRep_RepData();
	
};
