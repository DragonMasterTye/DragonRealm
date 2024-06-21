// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DR_Character.generated.h"

class UDR_InteractionComponent;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class DRAGONREALM_API ADR_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADR_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDR_InteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* AbilityArrowComponent;

	// Variables / Properties
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> PrimaryAttackProjectileClass;
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> UltProjectileClass;
	UPROPERTY(EditAnywhere, Category="Abilities")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category="Abilities")
	FName AbilitySpawnSocket = "Magic_R_Socket";

	// Timers
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_UltAbility;
	
	// Movement Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	//void Turn(float Value);
	//void LookUp(float Value);

	// Abilities
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void UltAbility();
	void UltAbility_TimeElapsed();

	// Interaction
	void PrimaryInteract();

	// Utilities
	FRotator CalculateAimRotation();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
