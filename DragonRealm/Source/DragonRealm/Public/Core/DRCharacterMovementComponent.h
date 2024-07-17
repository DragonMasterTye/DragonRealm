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
		enum CompressedFlags
		{
			FLAG_Sprint			= 0x10,
			FLAG_Dash			= 0x20,
			FLAG_Custom_2		= 0x40,
			FLAG_Custom_3		= 0x80,
		};

		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint:1;
		uint8 Saved_bWallIsOnRight:1;

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

	// Overriden Default Functions
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;

	// Flags
	bool Safe_bWantsToSprint;

	// WallRun working Variables
	bool Safe_bWallIsOnRight;

	// Properties
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Walk_MaxWalkSpeed;

	// CCMC Helpers
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	// Wall Run Checks
	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const { return IsCustomMovementMode(CMOVE_WallRun); }
	UFUNCTION(BlueprintPure)
	bool IsWallOnRight() const { return Safe_bWallIsOnRight; }
	
	// Wall Run Parameters
	UPROPERTY(EditDefaultsOnly) float MinWallRunSpeed=200.f;
	UPROPERTY(EditDefaultsOnly) float MaxWallRunSpeed=800.f;
	UPROPERTY(EditDefaultsOnly) float MaxVerticalWallRunSpeed=200.f;
	UPROPERTY(EditDefaultsOnly) float WallRunPullAwayAngle=75;
	UPROPERTY(EditDefaultsOnly) float WallAttractionForce = 200.f;
	UPROPERTY(EditDefaultsOnly) float MinWallRunHeight=50.f;
	UPROPERTY(EditDefaultsOnly) UCurveFloat* WallRunGravityScaleCurve;
	UPROPERTY(EditDefaultsOnly) float WallJumpOffForce = 300.f;

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
	// Wall Run
	private:
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);
//-------------------------------------------MOVEMENT FUNCTIONS-------------------------------------------
	
protected:
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

#pragma endregion
// CCMC
	
};
