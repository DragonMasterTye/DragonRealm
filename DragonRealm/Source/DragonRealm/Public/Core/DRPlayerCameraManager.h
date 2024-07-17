// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DRPlayerCameraManager.generated.h"


UCLASS()
class DRAGONREALM_API ADRPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	ADRPlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration;
	float CrouchBlendTime;
};
