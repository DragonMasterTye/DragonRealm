// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRLockOnComponent.generated.h"


class ADRPlayerCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Ctor
	UDRLockOnComponent();

	UFUNCTION(BlueprintCallable)
	void SearchForTargetables();

protected:

	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|LockOn")
	ADRPlayerCharacter* Owner;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|LockOn")
	float TicksPerSecond;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|LockOn")
	TArray<AActor*> TargetableActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|LockOn")
	AActor* TargetedActor;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	float ConeTraceDistance;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	float ConeTraceStartRadius;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	float ConeTraceEndRadius;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	int ConeTraceSteps;
	UPROPERTY()
	FVector ConeTraceOffset;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	TEnumAsByte<ECollisionChannel> ConeTraceChannel;

	// Timers
	FTimerHandle TimerHandle_SearchForTargetables;
	
	// Unreal Functions
	virtual void BeginPlay() override;

	// Functions

	void LockOn();
	void LockOff();
};
