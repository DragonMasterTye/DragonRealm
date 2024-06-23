// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DR_GameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;

namespace EEnvQueryStatus
{
	enum Type : int;
}

class UEnvQuery;
UCLASS()
class DRAGONREALM_API ADR_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ADR_GameModeBase();
	
	// Unreal Functions
	virtual void StartPlay() override;

protected:

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor>MinionClass;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	// Timer
	FTimerHandle TimerHandle_SpawnBots;

	// Functions
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
};
