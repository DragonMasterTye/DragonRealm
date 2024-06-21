// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DR_GameplayInterface.h"
#include "GameFramework/Actor.h"
#include "DR_ItemChest.generated.h"

UCLASS()
class DRAGONREALM_API ADR_ItemChest : public AActor, public IDR_GameplayInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADR_ItemChest();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetPitch;
	
	void Interact_Implementation(APawn* InstigatorPawn);

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
