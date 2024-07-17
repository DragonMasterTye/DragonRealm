// Copyright Landon Morrison 2024


#include "Core/DRPlayerCameraManager.h"

#include "Components/CapsuleComponent.h"
#include "Core/DRCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/DRPlayerCharacter.h"

ADRPlayerCameraManager::ADRPlayerCameraManager() : CrouchBlendDuration(0.2f)
{}

void ADRPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	// @TODO this functionality is tick dependant and breaks with Camera Lag on SpringArm, create a better system
	if (ADRPlayerCharacter* DRPlayerCharacter = Cast<ADRPlayerCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UDRCharacterMovementComponent* DRCharacterMovementComponent = DRPlayerCharacter->GetDRCharacterMovement();
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			DRCharacterMovementComponent->GetCrouchedHalfHeight() - DRPlayerCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector ActualOffset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (DRCharacterMovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			ActualOffset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += ActualOffset;
	}
}
