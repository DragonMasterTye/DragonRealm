// Copyright Landon Morrison 2024


#include "DR_GameModeBase.h"

#include "DR_AICharacter_Base.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "DR_AttributeComponent.h"
#include "DR_Character.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("DR.SpawnBots"), true, TEXT("Enable Spawning of bots via timer"), ECVF_Cheat);

ADR_GameModeBase::ADR_GameModeBase()
{
	SpawnTimerInterval = 2.f;
	RespawnDelay = 2.f;
}

void ADR_GameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ADR_GameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ADR_GameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	ADR_Character* PlayerChar = Cast<ADR_Character>(Victim);
	if(PlayerChar)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "OnRespawnPlayerTimerElapsed", PlayerChar->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

void ADR_GameModeBase::DR_KillAllAI()
{
	for(TActorIterator<ADR_AICharacter_Base> It(GetWorld()); It; ++It)
	{
		ADR_AICharacter_Base* Bot = *It;

		UDR_AttributeComponent* AttributeComponent = UDR_AttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); // @fixme update to player that input command
		}
	}
}

void ADR_GameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Error, TEXT("Bot spawning diasbled via CVAR: DR.SpawnBots"));
		return;
	}
	int32 NrOfAliveBots = 0;
	for(TActorIterator<ADR_AICharacter_Base> It(GetWorld()); It; ++It)
	{
		ADR_AICharacter_Base* Bot = *It;
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ADR_GameModeBase::OnQueryCompleted);
	}
}

void ADR_GameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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

void ADR_GameModeBase::OnRespawnPlayerTimerElapsed(AController* Controller)
{
	if(Controller)
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}
