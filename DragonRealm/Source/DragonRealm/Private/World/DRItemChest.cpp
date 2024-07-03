// Fill out your copyright notice in the Description page of Project Settings.


#include "World/DRItemChest.h"

#include "Net/UnrealNetwork.h"

// Ctor
ADRItemChest::ADRItemChest()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110.0f;
}

// Functions------------------------------------------
void ADRItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = true;
	OnRep_LidOpened();
}

void ADRItemChest::OnActorLoaded_Implementation()
{
	IDRGameplayInterface::OnActorLoaded_Implementation();
	OnRep_LidOpened();
}

void ADRItemChest::OnRep_LidOpened_Implementation()
{
	
}

// Replication
void ADRItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADRItemChest, bLidOpened);
}