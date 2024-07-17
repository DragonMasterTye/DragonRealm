// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DRCharacterMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"),
	CMOVE_Hang			UMETA(DisplayName = "Hang"),
	CMOVE_Climb			UMETA(DisplayName = "Climb"),
	CMOVE_MAX			UMETA(Hidden),
};

UCLASS()
class DRAGONREALM_API UDRCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

// SavedMove
#pragma region SavedMove
	// Structure for holding movement data to be sent over the network
	class FSavedMove_DRCharacter : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint:1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
#pragma endregion
// SavedMove

// Network Prediction
#pragma region Network Prediction
	// Overriding built-in move allocation to use our custom SavedMove
	class FNetworkPredictionData_Client_DRCharacter : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_DRCharacter(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
#pragma endregion
// Network Prediction	

// CCMC
#pragma region CustomCharacterMovementComponent
	
public:
	// Ctor
	UDRCharacterMovementComponent();

	// Getters
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Flags
	bool Safe_bWantsToSprint;

	// Properties
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Walk_MaxWalkSpeed;

//-------------------------------------------MOVEMENT FUNCTIONS-------------------------------------------
	// Sprint
	UFUNCTION(BlueprintCallable)
	void SprintToggle();
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
	// Crouch
	UFUNCTION(BlueprintCallable)
	void CrouchToggle();
	UFUNCTION(BlueprintCallable)
	void CrouchPressed();
	UFUNCTION(BlueprintCallable)
	void CrouchReleased();
//-------------------------------------------MOVEMENT FUNCTIONS-------------------------------------------
	
protected:
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

#pragma endregion
// CCMC
	
};
