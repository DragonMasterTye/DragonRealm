// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DRPlayerCharacter.generated.h"

class UDRActionComponent;
class ADRProjectile;
class UDRAttributeComponent;
class UDRInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class DRAGONREALM_API ADRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Ctor
	ADRPlayerCharacter();

	// Unreal Functions
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function
	UFUNCTION(Exec)
	void DR_HealSelf(float Amount = 100.f);

protected:

	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* SpawnTransformArrow;
	
	// Imaginary(Actor) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDRInteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDRAttributeComponent* AttributeComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDRActionComponent* ActionComponent;
	
	//  Properties
	UPROPERTY(EditAnywhere, Category="DR|Assignables")
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
	FName TimeOfHitParamName;

	// Unreal Functions
	virtual void PostInitializeComponents() override;
	virtual FVector GetPawnViewLocation() const override;
	
	// Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryInteract();
	UFUNCTION()
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float Delta, float ActualDelta);

	// Actions
	void StartSprint();
	void StopSprint();
	void PrimaryAction();
	void SecondaryAction();
	void UltimateAction();
};
