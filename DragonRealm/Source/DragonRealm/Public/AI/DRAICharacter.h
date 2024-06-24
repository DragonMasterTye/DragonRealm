// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DRAICharacter.generated.h"

class UDRWorldUserWidget;
class UDRAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class DRAGONREALM_API ADRAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADRAICharacter();

	// Functions
	UFUNCTION()
	bool IsAlive();

protected:

	// Imaginary Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDRAttributeComponent* AttributeComponent;

	// Properties
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeOfHitParamName;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	// UPROPERTY()
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
