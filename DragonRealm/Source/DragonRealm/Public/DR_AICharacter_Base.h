// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DR_AICharacter_Base.generated.h"

class UDR_AttributeComponent;
class UPawnSensingComponent;

UCLASS()
class DRAGONREALM_API ADR_AICharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADR_AICharacter_Base();

	// Functions
	UFUNCTION()
	bool IsAlive();

protected:

	// Imaginary Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDR_AttributeComponent* AttributeComponent;

	// Unreal Functions
	virtual void PostInitializeComponents() override;

	// Functions
	UFUNCTION(BlueprintNativeEvent)
	void OnPawnSeen(APawn* Pawn);

	
};
