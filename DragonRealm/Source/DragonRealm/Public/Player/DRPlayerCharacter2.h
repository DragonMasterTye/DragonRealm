// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Core/DRBaseCharacter2.h"
#include "DRPlayerCharacter2.generated.h"

struct FInputActionValue;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class DRAGONREALM_API ADRPlayerCharacter2 : public ADRBaseCharacter2
{
	GENERATED_BODY()

public:

	ADRPlayerCharacter2();

protected:

	/* Responsible for camera collisions and dynamic camera movement */
	UPROPERTY(VisibleAnywhere, Category = "DR|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	/* The main player Camera */
	UPROPERTY(VisibleAnywhere, Category = "DR|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/////////////////////////////////////////////////////////////////////////////////////////
	/* Player Input */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	/* Default Input Mappings */
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;
	// Locomotion Inputs
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_Move;
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_LookMouse;
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_LookGamepad;
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_Jump;
	// Other Inputs
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_Interact;
	/* Not sure how I'm going to use these yet
	/*UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_Sprint;
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_Dash;*/
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	UPROPERTY(EditDefaultsOnly, Category="DR|Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;

	// Locomotion Functions
	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void LookGamepad(const FInputActionValue& InputValue);
	
	void OnJumpStarted(const FInputActionValue& ActionValue);
	void OnJumpEnded(const FInputActionValue& ActionValue);

	// Attack Functions
	void PrimaryAttack(const FInputActionValue& InputValue);


	/* Player Input */
	///////////////////////////////////////////////////////////////////////////////////////////
};
