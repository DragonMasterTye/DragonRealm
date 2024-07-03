// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/DRAction.h"
#include "DRActionEffect.generated.h"


UCLASS()
class DRAGONREALM_API UDRActionEffect : public UDRAction
{
	GENERATED_BODY()

public:

	// Ctor
	UDRActionEffect();

	// Functions
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	UFUNCTION(BlueprintCallable, Category = "DR|Actions")
	float GetTimeRemaining() const;

protected:

	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|ActionEffects")
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|ActionEffects")
	float Period;

	// Timers
	FTimerHandle TimerHandle_PeriodHandle;
	FTimerHandle TimerHandle_DurationHandle;

	// Functions
	UFUNCTION(BlueprintNativeEvent, Category = "DR|ActionEffects")
	void ExecutePeriodEffect(AActor* Instigator);
	
};
