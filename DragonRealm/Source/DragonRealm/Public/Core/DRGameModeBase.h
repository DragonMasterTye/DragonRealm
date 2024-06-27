// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DRGameModeBase.generated.h"

class UDRSaveGame;
class UEnvQueryInstanceBlueprintWrapper;

namespace EEnvQueryStatus
{
	enum Type : int;
}

class UEnvQuery;
UCLASS()
class DRAGONREALM_API ADRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Ctor
	ADRGameModeBase();
	
	// Unreal Functions
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	// Functions
	virtual void OnActorKilled(AActor* Victim, AActor* Killer);
	
	// SaveGame
	UFUNCTION(BlueprintCallable, Category = "DR|SaveGame")
	void WriteSaveGame(FString InSaveGameName = "DRSaveGame");
	void LoadSaveGame();

	// Console Functions
	UFUNCTION(Exec)
	void DR_KillAllAI();

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
	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay;

	// SaveGame
	FString SaveSlotName;
	UPROPERTY()
	UDRSaveGame* CurrentSaveGame;

	// Timer
	FTimerHandle TimerHandle_SpawnBots;

	// Functions
	UFUNCTION()
	void SpawnBotTimerElapsed();
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnRespawnPlayerTimerElapsed(AController* Controller);
	
};
