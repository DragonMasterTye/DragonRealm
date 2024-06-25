// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DRAICharacter.generated.h"

class UDRActionComponent;
class UDRWorldUserWidget;
class UDRAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class DRAGONREALM_API ADRAICharacter : public ACharacter
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
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDRAttributeComponent* AttributeComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDRActionComponent* ActionComponent;

	// Properties
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeOfHitParamName;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UDRWorldUserWidget* ActiveHealthBar;

	// Unreal Functions
	virtual void PostInitializeComponents() override;

	// Functions
	UFUNCTION(BlueprintNativeEvent)
	void OnPawnSeen(APawn* Pawn);
	UFUNCTION()
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float Delta);

	// Utilities
	void SetTargetActor(AActor* NewTarget);
	
};
