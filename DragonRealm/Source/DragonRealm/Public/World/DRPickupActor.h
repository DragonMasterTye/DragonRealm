// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Core/DRGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "DRPickupActor.generated.h"

class USphereComponent;
UCLASS()
class DRAGONREALM_API ADRPickupActor : public AActor, public IDRGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRPickupActor();

	// Overrides
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	// Properties
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float RespawnTime = 5.f;

	// Timers
	FTimerHandle TimerHandle_RespawnTimer;

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowPickup();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideAndCooldownPickup();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPickupState(bool bNewState);
	
};


