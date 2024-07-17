// Copyright Landon Morrison 2024


#include "Core/DRCharacterMovementComponent.h"

#include "GameFramework/Character.h"

// SavedMove
#pragma region SavedMove
// Attempting to combine move to save bandwidth
bool UDRCharacterMovementComponent::FSavedMove_DRCharacter::CanCombineWith(const FSavedMovePtr& NewMove,
																			ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_DRCharacter* NewDRMove = static_cast<FSavedMove_DRCharacter*>(NewMove.Get());
	
	if(Saved_bWantsToSprint != NewDRMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

// Reset all related variables
void UDRCharacterMovementComponent::FSavedMove_DRCharacter::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

// 8 Flags for important movement mode switches (jump, sprint, fly...)
uint8 UDRCharacterMovementComponent::FSavedMove_DRCharacter::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if(Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}

// Sets Saved variables using our current Safe Variables (Owning Client snapshot to send to Server)
void UDRCharacterMovementComponent::FSavedMove_DRCharacter::SetMoveFor(ACharacter* C, float InDeltaTime,
																		FVector const& NewAccel,
																		FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UDRCharacterMovementComponent* CharacterMovement = Cast<UDRCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

// Sets Safe variables using the received Saved variables (Received Snapshot is being applied to actual variables)
void UDRCharacterMovementComponent::FSavedMove_DRCharacter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UDRCharacterMovementComponent* CharacterMovement = Cast<UDRCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

#pragma endregion
// SavedMove

// Network Prediction
#pragma region Network Prediction

UDRCharacterMovementComponent::FNetworkPredictionData_Client_DRCharacter::FNetworkPredictionData_Client_DRCharacter(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{}

// Overriding built-in move allocation to use our custom SavedMove
FSavedMovePtr UDRCharacterMovementComponent::FNetworkPredictionData_Client_DRCharacter::AllocateNewMove()
{
	return MakeShared<FSavedMove_DRCharacter>();
}
#pragma endregion
// Network Prediction

// CCMC
#pragma region CustomCharacterMovementComponent

// Ctor for CCMC (CustomCharacterMovementComponent)
UDRCharacterMovementComponent::UDRCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

// Set the CMC to use the custom prediction data
FNetworkPredictionData_Client* UDRCharacterMovementComponent::GetPredictionData_Client() const
{	
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UDRCharacterMovementComponent* MutableThis = const_cast<UDRCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_DRCharacter(*this);

		//@TODO test these values to find the best magic numbers
		//@see https://dev.epicgames.com/documentation/en-us/unreal-engine/testing-and-optimizing-your-content for testing
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f; // These Values only affect Simulated Proxies (Non-Owned Clients)
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f; // These Values only affect Simulated Proxies (Non-Owned Clients)
	}
	return ClientPredictionData;
}

// Decompress Flags to update movement state
void UDRCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_DRCharacter::FLAG_Custom_0) != 0;
}

// Called after 
void UDRCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

#pragma endregion
// CCMC

//-------------------------------------------MOVEMENT FUNCTIONS-------------------------------------------

// Sprint
void UDRCharacterMovementComponent::SprintToggle()
{
	Safe_bWantsToSprint = !Safe_bWantsToSprint;
}
void UDRCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}
void UDRCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

// Crouch
void UDRCharacterMovementComponent::CrouchToggle()
{
	bWantsToCrouch = !bWantsToCrouch;
}
void UDRCharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouch = true;
}
void UDRCharacterMovementComponent::CrouchReleased()
{
	bWantsToCrouch = false;	
}

//-------------------------------------------MOVEMENT FUNCTIONS-------------------------------------------
