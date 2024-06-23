// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DR_Character.generated.h"

class ADR_Projectile_Base;
class UDR_AttributeComponent;
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

	// Physical Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* AbilityArrowComponent;
	
	// Imaginary Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDR_InteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDR_AttributeComponent* AttributeComponent;
    

	// Variables / Properties
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<ADR_Projectile_Base> PrimaryAttackProjectileClass;
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<ADR_Projectile_Base> UltProjectileClass;
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<ADR_Projectile_Base> DashProjectileClass;
	UPROPERTY(EditAnywhere, Category="Abilities")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category="Abilities")
	FName AbilitySpawnSocket = "Magic_R_Socket";

	// Timers
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_UltAbility;
	FTimerHandle TimerHandle_DashAbility;
	
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
	void DashAbility();
	void DashAbility_TimeElapsed();

	// Interaction
	void PrimaryInteract();

	// Utilities
	FRotator CalculateAimRotation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
	void SpawnProjectile(TSubclassOf<ADR_Projectile_Base> ProjectileClass);

	// Functions
	UFUNCTION()
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDR_AttributeComponent* OwningComponent, float NewHealth, float Delta);

	// Unreal Functions
	virtual void PostInitializeComponents() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
