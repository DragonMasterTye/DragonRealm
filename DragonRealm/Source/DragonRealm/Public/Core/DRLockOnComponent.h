// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRLockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Ctor
	UDRLockOnComponent();

protected:

	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|LockOn")
	float TicksPerSecond;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|LockOn")
	AActor* TargetedActor;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	float TraceRadius;
	UPROPERTY(EditDefaultsOnly, Category = "DR|LockOn")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	// Timers
	FTimerHandle TimerHandle_SearchForLock;
	
	// Unreal Functions
	virtual void BeginPlay() override;

	// Functions
	void SearchForLock();
	void LockOn();
	void LockOff();
};
