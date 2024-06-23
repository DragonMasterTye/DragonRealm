// Copyright Landon Morrison 2024


#include "DR_AICharacter_Base.h"

// Sets default values
ADR_AICharacter_Base::ADR_AICharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADR_AICharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADR_AICharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADR_AICharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

