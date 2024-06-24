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

protected:
	// Unreal Functions
	virtual void BeginPlay() override;

	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* AbilityArrowComponent;
	
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

	// Timers
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_UltAbility;
	FTimerHandle TimerHandle_DashAbility;
	
	// Movement Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	//void Turn(float Value);
	//void LookUp(float Value);

	// Actions
	void StartSprint();
	void StopSprint();
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void UltAbility();
	void UltAbility_TimeElapsed();
	void DashAbility();
	void DashAbility_TimeElapsed();

	// Interaction
	void PrimaryInteract();

	// Utilities
	FRotator CalculateAimRotation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
	void SpawnProjectile(TSubclassOf<ADRProjectile> ProjectileClass);

	// Functions
	UFUNCTION()
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float Delta);

	// Unreal Functions
	virtual void PostInitializeComponents() override;
	virtual FVector GetPawnViewLocation() const override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void DR_HealSelf(float Amount = 100.f);

};
