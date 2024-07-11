// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Core/DRBaseCharacter.h"
#include "GameFramework/Character.h"
#include "DRAICharacter.generated.h"

class UDRActionComponent;
class UDRWorldUserWidget;
class UDRAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class DRAGONREALM_API ADRAICharacter : public ADRBaseCharacter
{
	GENERATED_BODY()

public:
	// Ctor
	ADRAICharacter();

	// Functions
	UFUNCTION()
	bool IsAlive();

protected:

	// Imaginary(Actor) Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	// Properties
	UPROPERTY(VisibleAnywhere, Category = "DR|Effects")
	FName TimeOfHitParamName;
	UPROPERTY(EditDefaultsOnly, Category = "DR|UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UDRWorldUserWidget* ActiveHealthBar;
	UPROPERTY()
	int32 EXPToGrant;
	UPROPERTY(EditAnywhere, Category = "DR|EXP")
	int32 MaxEXP;
	UPROPERTY(EditAnywhere, Category = "DR|EXP")
	int32 MinEXP;

	// Unreal Functions
	virtual void PostInitializeComponents() override;

	// Functions
	UFUNCTION(BlueprintNativeEvent)
	void OnPawnSeen(APawn* Pawn);
	UFUNCTION()
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float Delta, float ActualDelta);

	// Utilities
	void SetTargetActor(AActor* NewTarget);
	
};
