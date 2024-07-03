// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DRSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;
	UPROPERTY()
	FTransform ActorTransform;
	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class DRAGONREALM_API UDRSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 EXP;
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
