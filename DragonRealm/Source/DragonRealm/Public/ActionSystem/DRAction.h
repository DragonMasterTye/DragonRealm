// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "DRAction.generated.h"

class UDRActionComponent;
class UWorld;
UCLASS(Blueprintable)
class DRAGONREALM_API UDRAction : public UObject
{
	GENERATED_BODY()

public:

	// Unreal Functions
	virtual UWorld* GetWorld() const override;

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "DR|Assignables")
	FName ActionName;

	// Functions
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	bool CanStart(AActor* Instigator); // Since this is a UObject, we don't already have an Instigator so this is a safe name
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	void StartAction(AActor* Instigator); 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DR|Actions")
	void StopAction(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "DR|Actions")
	bool IsRunning() const;

protected:

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DR|Tags")
	FGameplayTagContainer GrantsTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DR|Tags")
	FGameplayTagContainer BlockedByTags;
	bool bIsRunning;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	UDRActionComponent* GetOwningComponent() const;
	
};
