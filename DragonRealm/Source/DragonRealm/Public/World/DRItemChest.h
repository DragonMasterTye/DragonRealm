// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DRGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "DRItemChest.generated.h"

UCLASS()
class DRAGONREALM_API ADRItemChest : public AActor, public IDRGameplayInterface
{
	GENERATED_BODY()

public:	
	// Ctor
	ADRItemChest();

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetPitch;

	// Functions
	virtual void Interact_Implementation(APawn* InstigatorPawn);
	virtual void OnActorLoaded_Implementation() override;

protected:
	
	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Replication
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", VisibleAnywhere, BlueprintReadOnly, SaveGame) // RepNotify
	bool bLidOpened;
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_LidOpened();
	
};
