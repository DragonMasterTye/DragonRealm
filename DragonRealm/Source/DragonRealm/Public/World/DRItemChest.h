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
	// Sets default values for this actor's properties
	ADRItemChest();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetPitch;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
