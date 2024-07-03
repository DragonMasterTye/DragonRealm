// Copyright Landon Morrison 2024


#include "Player/DRPlayerController.h"

#include "Blueprint/UserWidget.h"

// Unreal Functions
void ADRPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}

void ADRPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}

void ADRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InGameMenu", IE_Pressed, this, &ADRPlayerController::ToggleInGameMenu);
}

// Functions-----------------
void ADRPlayerController::ToggleInGameMenu()
{
	if(InGameMenu && InGameMenu->IsInViewport())
	{
		InGameMenu->RemoveFromParent();
		InGameMenu = nullptr;

		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		InGameMenu = CreateWidget<UUserWidget>(this,InGameMenuClass);
		if(InGameMenu)
		{
			InGameMenu->AddToViewport(100);

			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
		}
	}
}
