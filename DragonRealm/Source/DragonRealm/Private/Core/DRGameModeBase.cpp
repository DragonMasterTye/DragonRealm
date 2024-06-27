// Copyright Landon Morrison 2024


#include "Core/DRGameModeBase.h"

#include "AI/DRAICharacter.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "Player/DRPlayerCharacter.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("DR.SpawnBots"), false, TEXT("Enable Spawning of bots via timer"), ECVF_Cheat);

ADRGameModeBase::ADRGameModeBase()
{
	SpawnTimerInterval = 2.f;
	RespawnDelay = 2.f;
}

void ADRGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ADRGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ADRGameModeBase::OnQueryCompleted);
	}
}

void ADRGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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
		if(ensureMsgf(MinionClass, TEXT("No MinionClass Assigned in GameModeBase!")))
		{
			GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0] + (0,0,100), FRotator::ZeroRotator);
		}

		DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false);
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
