// Fill out your copyright notice in the Description page of Project Settings.


#include "World/DRItemChest.h"

// Sets default values
ADRItemChest::ADRItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110.0f;
}

// Called when the game starts or when spawned
void ADRItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADRItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADRItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(110, 0, 0));
}