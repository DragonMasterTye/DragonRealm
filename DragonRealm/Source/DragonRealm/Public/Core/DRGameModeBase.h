// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DRGameModeBase.generated.h"

class UDRMonsterData;
class UDRSaveGame;
class UDataTable;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
namespace EEnvQueryStatus
{
	enum Type : int;
}

// DataTable Struct
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		SpawnWeight = 1.f;
		SpawnCost = 5.f;
		KillReward = 10.f;
	}
	
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterDataID;
	// UDRMonsterData* MonsterData; // Standard Method
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
	
};


// GameMode
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
	UPROPERTY(EditDefaultsOnly, Category = "DR|AI")
	float SpawnTimerInterval;
	UPROPERTY(EditDefaultsOnly, Category = "DR|AI")
	UCurveFloat* DifficultyCurve;
	UPROPERTY(EditDefaultsOnly, Category = "DR|AI")
	TSubclassOf<AActor> MinionClass;
	UPROPERTY(EditDefaultsOnly, Category = "DR|AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, Category = "DR|AI")
	UDataTable* MonsterTable;
	UPROPERTY(EditDefaultsOnly, Category = "DR|Respawn")
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
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnRespawnPlayerTimerElapsed(AController* Controller);
	void OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation);
	
};
