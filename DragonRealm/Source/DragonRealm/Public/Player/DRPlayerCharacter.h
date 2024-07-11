// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DRBaseCharacter.h"
#include "GameFramework/Character.h"
#include "DRPlayerCharacter.generated.h"

struct FInputActionValue;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;
class UDRLockOnComponent;
class UDRActionComponent;
class ADRProjectile;
class UDRAttributeComponent;
class UDRInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class DRAGONREALM_API ADRPlayerCharacter : public ADRBaseCharacter
{
	GENERATED_BODY()

public:
	// Ctor
	ADRPlayerCharacter();

	// Unreal Functions
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Utility for testing healing
	UFUNCTION(Exec)
	void DR_HealSelf(float Amount = 100.f);

	// Getters
	UFUNCTION()
	UCameraComponent* GetCamera() const;

protected:

	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	
	// Imaginary(Actor) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDRInteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDRLockOnComponent* LockOnComponent;
	
	//  Properties
	/*UPROPERTY(EditAnywhere, Category="DR|Assignables")
	TSubclassOf<ADRProjectile> PrimaryAttackProjectileClass;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	TSubclassOf<ADRProjectile> UltProjectileClass;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	TSubclassOf<ADRProjectile> DashProjectileClass;
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category="Abilities")
	FName AbilitySpawnSocket;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeOfHitParamName;*/

	// Unreal Functions
	virtual void PostInitializeComponents() override;
	virtual FVector GetPawnViewLocation() const override;
	
	// Movement
	void Move(const FInputActionInstance& Instance);

	// Camera Control
	void LookMouse(const FInputActionValue& InputValue);
	void LookGamepad(const FInputActionValue& InputValue);

	// Interaction
	void PrimaryInteract();
	

	// Attributes
	UFUNCTION()
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float DesiredDelta, float ActualDelta);

	// Actions
	void StartSprint();
	void StopSprint();
	void PrimaryAction();
	void SecondaryAction();
	void UltimateAction();

	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_LookMouse;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_LookGamepad;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Interact;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Sprint;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_SecondaryAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_UltimateAction;
};
