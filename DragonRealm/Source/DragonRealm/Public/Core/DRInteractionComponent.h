// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRInteractionComponent.generated.h"


class UDRWorldUserWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Ctor
	UDRInteractionComponent();

	// Unreal Functions
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Functions
	void PrimaryInteract();

protected:

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "DR|Interaction")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category = "DR|Interaction")
	float TraceRadius;
	UPROPERTY(EditDefaultsOnly, Category = "DR|Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel;
	UPROPERTY()
	AActor* FocusedActor;
	UPROPERTY(EditDefaultsOnly, Category = "DR|UI")
	TSubclassOf<UDRWorldUserWidget> DefaultInteractionWidgetClass;
	UPROPERTY()
	UDRWorldUserWidget* DefaultInteractionWidget;
	
	// Unreal Functions
	virtual void BeginPlay() override;

	// Functions
	void FindBestInteractable();

	// Replication Functions
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocusedActor);
			
};
