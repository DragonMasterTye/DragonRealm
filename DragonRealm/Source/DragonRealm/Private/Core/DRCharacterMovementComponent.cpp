// Copyright Landon Morrison 2024


#include "Core/DRCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Core/DRBaseCharacter.h"
#include "GameFramework/Character.h"

// SavedMove
#pragma region SavedMove
UDRCharacterMovementComponent::FSavedMove_DRCharacter::FSavedMove_DRCharacter()
{
	Saved_bWantsToSprint = 0;
}

// Attempting to combine move to save bandwidth
bool UDRCharacterMovementComponent::FSavedMove_DRCharacter::CanCombineWith(const FSavedMovePtr& NewMove,
																			ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_DRCharacter* NewDRMove = static_cast<FSavedMove_DRCharacter*>(NewMove.Get());
	
	if(Saved_bWantsToSprint != NewDRMove->Saved_bWantsToSprint)
	{
		return false;
	}
	if(Saved_bWallIsOnRight != NewDRMove->Saved_bWallIsOnRight)
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
	Saved_bWallIsOnRight = 0;
	Saved_bPressedDRJump = 0;

	Saved_bTransitionFinished = 0;
}

// 8 Flags for important movement mode switches (jump, sprint, fly...)
uint8 UDRCharacterMovementComponent::FSavedMove_DRCharacter::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if(Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	if(Saved_bPressedDRJump) Result |= FLAG_JumpPressed;


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
	Saved_bWallIsOnRight = CharacterMovement->Safe_bWallIsOnRight;

	Saved_bPressedDRJump = CharacterMovement->DROwningCharacter->bPressedDRJump;
}

// Sets Safe variables using the received Saved variables (Received Snapshot is being applied to actual variables)
void UDRCharacterMovementComponent::FSavedMove_DRCharacter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UDRCharacterMovementComponent* CharacterMovement = Cast<UDRCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
	CharacterMovement->Safe_bWallIsOnRight = Saved_bWallIsOnRight;

	CharacterMovement->DROwningCharacter->bPressedDRJump = Saved_bPressedDRJump;
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

void UDRCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	DROwningCharacter = Cast<ADRBaseCharacter>(GetOwner());
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

bool UDRCharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround();
}

bool UDRCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

float UDRCharacterMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && Safe_bWantsToSprint && !IsCrouching()) return Sprint_MaxWalkSpeed;
	
	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();

	switch (CustomMovementMode)
	{
	case CMOVE_WallRun:
		return MaxWallRunSpeed;
	case CMOVE_Hang:
		return 0.f;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

float UDRCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_WallRun:
		return 0.f;
	case CMOVE_Hang:
		return 0.f;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

void UDRCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// Wall Run
	if (IsFalling())
	{
		TryWallRun();
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UDRCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

void UDRCharacterMovementComponent::OnClientCorrectionReceived(FNetworkPredictionData_Client_Character& ClientData,
	float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName,
	bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection)
{
	Super::OnClientCorrectionReceived(ClientData, TimeStamp, NewLocation, NewVelocity, NewBase, NewBaseBoneName,
									  bHasBase, bBaseRelativePosition,
									  ServerMovementMode, ServerGravityDirection);

	CorrectionCount++;
}

void UDRCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
		break;
	case CMOVE_WallRun:
		PhysWallRun(deltaTime, Iterations);
		break;
	case CMOVE_Hang:
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}

void UDRCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	
	if (IsFalling())
	{
		bOrientRotationToMovement = true;
	}

	if (IsWallRunning() && GetOwnerRole() == ROLE_SimulatedProxy)
	{
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector End = Start + UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
		auto Params = DROwningCharacter->GetIgnoreCharacterParams();
		FHitResult WallHit;
		Safe_bWallIsOnRight = GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
	}
}

bool UDRCharacterMovementComponent::ServerCheckClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel,
	const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, UPrimitiveComponent* ClientMovementBase,
	FName ClientBaseBoneName, uint8 ClientMovementMode)
{
	if (GetCurrentNetworkMoveData()->NetworkMoveType == FCharacterNetworkMoveData::ENetworkMoveType::NewMove)
	{
		float LocationError = FVector::Dist(UpdatedComponent->GetComponentLocation(), ClientWorldLocation);
		GEngine->AddOnScreenDebugMessage(6, 100.f, FColor::Yellow, FString::Printf(TEXT("Loc: %s"), *ClientWorldLocation.ToString()));
		AccumulatedClientLocationError += LocationError * DeltaTime;
	}
	
	return Super::ServerCheckClientError(ClientTimeStamp, DeltaTime, Accel, ClientWorldLocation, RelativeClientLocation,
	                                     ClientMovementBase,
	                                     ClientBaseBoneName, ClientMovementMode);
}

void UDRCharacterMovementComponent::CallServerMovePacked(const FSavedMove_Character* NewMove,
	const FSavedMove_Character* PendingMove, const FSavedMove_Character* OldMove)
{
	// Get storage container we'll be using and fill it with movement data
	FCharacterNetworkMoveDataContainer& MoveDataContainer = GetNetworkMoveDataContainer();
	MoveDataContainer.ClientFillNetworkMoveData(NewMove, PendingMove, OldMove);

	// Reset bit writer without affecting allocations
	FBitWriterMark BitWriterReset;
	BitWriterReset.Pop(DRServerMoveBitWriter);

	// 'static' to avoid reallocation each invocation
	static FCharacterServerMovePackedBits PackedBits;
	UNetConnection* NetConnection = CharacterOwner->GetNetConnection();	


	{
		// Extract the net package map used for serializing object references.
		DRServerMoveBitWriter.PackageMap = NetConnection ? ToRawPtr(NetConnection->PackageMap) : nullptr;
	}

	if (DRServerMoveBitWriter.PackageMap == nullptr)
	{
		UE_LOG(LogNetPlayerMovement, Error, TEXT("CallServerMovePacked: Failed to find a NetConnection/PackageMap for data serialization!"));
		return;
	}

	// Serialize move struct into a bit stream
	if (!MoveDataContainer.Serialize(*this, DRServerMoveBitWriter, DRServerMoveBitWriter.PackageMap) || DRServerMoveBitWriter.IsError())
	{
		UE_LOG(LogNetPlayerMovement, Error, TEXT("CallServerMovePacked: Failed to serialize out movement data!"));
		return;
	}

	// Copy bits to our struct that we can NetSerialize to the server.
	PackedBits.DataBits.SetNumUninitialized(DRServerMoveBitWriter.GetNumBits());
	
	check(PackedBits.DataBits.Num() >= DRServerMoveBitWriter.GetNumBits());
	FMemory::Memcpy(PackedBits.DataBits.GetData(), DRServerMoveBitWriter.GetData(), DRServerMoveBitWriter.GetNumBytes());

	TotalBitsSent += PackedBits.DataBits.Num();

	// Send bits to server!
	ServerMovePacked_ClientSend(PackedBits);

	MarkForClientCameraUpdate();
}

bool UDRCharacterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsWallRunning();
}

bool UDRCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	bool bWasWallRunning = IsWallRunning();
	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FVector Start = UpdatedComponent->GetComponentLocation();
			FVector CastDelta = UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
			FVector End = Safe_bWallIsOnRight ? Start + CastDelta : Start - CastDelta;
			auto Params = DROwningCharacter->GetIgnoreCharacterParams();
			FHitResult WallHit;
			GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
			Velocity += WallHit.Normal * WallJumpOffForce;
		}

		return true;
	}
	return false;
}

bool UDRCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool UDRCharacterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

// Decompress Flags to update movement state
void UDRCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_DRCharacter::FLAG_Sprint) != 0;
}

void UDRCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
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

bool UDRCharacterMovementComponent::TryWallRun()
{
	if (!IsFalling()) return false;
	if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2)) return false;
	if (Velocity.Z < -MaxVerticalWallRunSpeed) return false;
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector LeftEnd = Start - UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
	FVector RightEnd = Start + UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
	auto Params = DROwningCharacter->GetIgnoreCharacterParams();
	FHitResult FloorHit, WallHit;
	// Check Player Height
	if (GetWorld()->LineTraceSingleByProfile(FloorHit, Start, Start + FVector::DownVector *
		(CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + MinWallRunHeight), "BlockAll", Params))
	{
		return false;
	}
	
	// Left Cast
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, LeftEnd, "BlockAll", Params);
	if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
	{
		Safe_bWallIsOnRight = false;
	}
	// Right Cast
	else
	{
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, RightEnd, "BlockAll", Params);
		if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
		{
			Safe_bWallIsOnRight = true;
		}
		else
		{
			return false;
		}
	}
	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
	if (ProjectedVelocity.SizeSquared2D() < pow(MinWallRunSpeed, 2)) return false;
	
	// Passed all conditions
	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z, 0.f, MaxVerticalWallRunSpeed);
	SetMovementMode(MOVE_Custom, CMOVE_WallRun);

	return true;
}

void UDRCharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
{
	// Early out if we have bad data
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}
	
	bJustTeleported = false;
	float remainingTime = deltaTime;
	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector CastDelta = UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
		FVector End = Safe_bWallIsOnRight ? Start + CastDelta : Start - CastDelta;
		auto Params = DROwningCharacter->GetIgnoreCharacterParams();
		float SinPullAwayAngle = FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));
		FHitResult WallHit;
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
		bool bWantsToPullAway = WallHit.IsValidBlockingHit() && !Acceleration.IsNearlyZero() && (Acceleration.GetSafeNormal() | WallHit.Normal) > SinPullAwayAngle;
		if (!WallHit.IsValidBlockingHit() || bWantsToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}
		
		// Clamp Acceleration
		Acceleration = FVector::VectorPlaneProject(Acceleration, WallHit.Normal);
		Acceleration.Z = 0.f;
		
		// Apply acceleration
		CalcVelocity(timeTick, 0.f, false, GetMaxBrakingDeceleration());
		Velocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
		float TangentAccel = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal2D();
		bool bVelUp = Velocity.Z > 0.f;
		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelUp ? 0.f : TangentAccel) * timeTick;
		if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2) || Velocity.Z < -MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}
		
		// Compute move parameters
		const FVector Delta = timeTick * Velocity; // dx = v * dt
		const bool bZeroDelta = Delta.IsNearlyZero();
		if ( bZeroDelta )
		{
			remainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);
			FVector WallAttractionDelta = -WallHit.Normal * WallAttractionForce * timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);
		}
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt
	}

	
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector CastDelta = UpdatedComponent->GetRightVector() * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
	FVector End = Safe_bWallIsOnRight ? Start + CastDelta : Start - CastDelta;
	auto Params = DROwningCharacter->GetIgnoreCharacterParams();
	FHitResult FloorHit, WallHit;
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
	GetWorld()->LineTraceSingleByProfile(FloorHit, Start, Start + FVector::DownVector * (CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + MinWallRunHeight * .5f), "BlockAll", Params);
	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2))
	{
		SetMovementMode(MOVE_Falling);
	}
}

//-------------------------------------------MOVEMENT FUNCTIONS-------------------------------------------
