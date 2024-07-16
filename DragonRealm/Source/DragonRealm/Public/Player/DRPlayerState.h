// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DRPlayerState.generated.h"

class UDRAbilitySystemComponent;
class UDRSaveGame;
class ADRPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEXPChanged, ADRPlayerState*, PlayerState, int32, NewEXP, int32, Delta);

UCLASS()
class DRAGONREALM_API ADRPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ADRPlayerState();

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRAbilitySystemComponent* DRASC;

	// Properties
	UPROPERTY(BlueprintAssignable, Category = "DR|Events")
	FOnEXPChanged OnEXPChanged;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "DR|EXP")
	int32 GetEXP() const;
	UFUNCTION(BlueprintCallable, Category = "DR|EXP")
	void AddExp(int32 EXPToAdd);
	UFUNCTION(BlueprintCallable, Category = "DR|EXP")
	void RemoveExp(int32 EXPToRemove);

	// SaveGame
	UFUNCTION(BlueprintNativeEvent, Category = "DR|SaveGame")
	void SavePlayerState(UDRSaveGame* SaveObject);
	UFUNCTION(BlueprintNativeEvent, Category = "DR|SaveGame")
	void LoadPlayerState(UDRSaveGame* SaveObject);
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "DR|EXP")
	int32 EXP;
};
