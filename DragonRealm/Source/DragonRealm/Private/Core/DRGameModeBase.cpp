// Copyright Landon Morrison 2024


#include "Core/DRGameModeBase.h"

#include "AI/DRAICharacter.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "AI/Data/DRMonsterData.h"
#include "Core/DRGameplayInterface.h"
#include "Core/DRSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DRPlayerCharacter.h"
#include "Player/DRPlayerState.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("DR.SpawnBots"), false, TEXT("Enable Spawning of bots via timer"), ECVF_Cheat);

// Ctor
ADRGameModeBase::ADRGameModeBase()
{
	SpawnTimerInterval = 2.f;
	RespawnDelay = 2.f;
	SaveSlotName = "DRSaveGame";
}

// Unreal Functions
void ADRGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ADRGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ADRGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ADRGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ADRPlayerState* PS = NewPlayer->GetPlayerState<ADRPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

// Functions------------------------------------------------------
void ADRGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	ADRPlayerCharacter* PlayerChar = Cast<ADRPlayerCharacter>(Victim);
	if(PlayerChar)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "OnRespawnPlayerTimerElapsed", PlayerChar->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

// Save Game
void ADRGameModeBase::WriteSaveGame(FString InSaveGameName /* = "DRSaveGame" */)
{
	SaveSlotName = InSaveGameName;

	// Save PlayerStates
	for(int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ADRPlayerState* PS = Cast<ADRPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // SingePlayer only atm @TODO make multiplayer
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Save ActorData
	for(FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only Interested in "Gameplay Actors"
		if(!Actor->Implements<UDRGameplayInterface>())
		{
			continue;
		}
		
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetTransform();
		
		FMemoryWriter MemoryWriter(ActorData.ByteData); // Pass in the area to fill with dara from actor
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true; // Find only variables with UPROPERTY(SaveGame)
		
		Actor->Serialize(Archive); // Convert Actor's SaveGame UPROPERTIES into binary array

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

void ADRGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame = Cast<UDRSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data"));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data"));

		// Populate ActorData
		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only Interested in "Gameplay Actors"
			if(!Actor->Implements<UDRGameplayInterface>())
			{
				continue;
			}

			for(FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.ActorTransform);

					FMemoryReader MemoryReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true; // Find only variables with UPROPERTY(SaveGame)
		
					Actor->Serialize(Archive); // Convert binary array back into Actor's variables

					IDRGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
	}
	else// No SaveGameObject in that slot, create a new one
	{
		CurrentSaveGame = Cast<UDRSaveGame>(UGameplayStatics::CreateSaveGameObject(UDRSaveGame::StaticClass()));
		
		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data"));
	}
	// Done Loading SaveGameObject
}

void ADRGameModeBase::DR_KillAllAI()
{
	for(TActorIterator<ADRAICharacter> It(GetWorld()); It; ++It)
	{
		ADRAICharacter* Bot = *It;

		UDRAttributeComponent* AttributeComponent = UDRAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); // @fixme update to player that input command
		}
	}
}

void ADRGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Error, TEXT("Bot spawning diasbled via CVAR: DR.SpawnBots"));
		return;
	}
	int32 NrOfAliveBots = 0;
	for(TActorIterator<ADRAICharacter> It(GetWorld()); It; ++It)
	{
		ADRAICharacter* Bot = *It;
		if(Bot->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	int MaxBotCount = 10;
	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if(NrOfAliveBots >= MaxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if(ensureAlways(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ADRGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ADRGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnBot EQS Query Failed!"));
		return;
	}

	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if(Locations.IsValidIndex(0))
	{
		/*if(ensureMsgf(MinionClass, TEXT("No MinionClass Assigned in GameModeBase!")))
		{
			GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0] + (0,0,100), FRotator::ZeroRotator);
		}*/
		if(MonsterTable)
		{
			TArray<FMonsterInfoRow*> MonsterRows;
			MonsterTable->GetAllRows("", MonsterRows);

			int32 RandomIndex = FMath::RandRange(0, MonsterRows.Num()-1);
			FMonsterInfoRow* SelectedInfoRow = MonsterRows[RandomIndex];

			GetWorld()->SpawnActor<AActor>(SelectedInfoRow->MonsterData->MonsterClass, Locations[0] + (0,0,100), FRotator::ZeroRotator);
		}
		else
		{
			GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0] + (0,0,100), FRotator::ZeroRotator);
		}
		
		//DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false);
	}
}

void ADRGameModeBase::OnRespawnPlayerTimerElapsed(AController* Controller)
{
	if(Controller)
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}
