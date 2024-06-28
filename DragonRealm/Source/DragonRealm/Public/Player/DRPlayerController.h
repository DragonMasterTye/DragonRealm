// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DRPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, PlayerState);

UCLASS()
class DRAGONREALM_API ADRPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "DR|UI")
	TSubclassOf<UUserWidget> InGameMenuClass;
	UPROPERTY()
	UUserWidget* InGameMenu;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;

	// Unreal Functions
	virtual void BeginPlayingState() override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupInputComponent() override;

	// Functions
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
	UFUNCTION(BlueprintCallable)
	void ToggleInGameMenu();
	
};
