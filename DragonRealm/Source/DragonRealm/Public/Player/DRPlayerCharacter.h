// Copyright Landon Morrison 2024


#pragma once

#include "CoreMinimal.h"
#include "Core/DRBaseCharacter.h"
#include "GameFramework/Character.h"
#include "DRPlayerCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDRPlayerCharacter, Log, All);

struct FInputActionValue;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;
class UDRBaseLockOnComponent;
class UDRActionComponent;
class ADRProjectile;
class UDRAttributeComponent;
class UDRBaseInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS(config=Game)
class DRAGONREALM_API ADRPlayerCharacter : public ADRBaseCharacter
{
	GENERATED_BODY()

public:
	
	ADRPlayerCharacter();
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Jump;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Move;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Look;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	/* DEPRECATED
public:
	// Ctor
	ADRPlayerCharacter();

	// Unreal Functions
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Utility for testing healing
	//UFUNCTION(Exec)
	//void DR_HealSelf(float Amount = 100.f);

	// Getters
	UFUNCTION()
	FORCEINLINE UCameraComponent* GetCamera() const { return CameraComponent; };
	UFUNCTION()
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArmComponent; };

protected:

	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	
	// Imaginary(Actor) Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDRBaseInteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDRBaseLockOnComponent* LockOnComponent;
	
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
	FName TimeOfHitParamName;

	// Unreal Functions
	virtual FVector GetPawnViewLocation() const override;
	
	// Movement
	void Move(const FInputActionInstance& Instance);

	// Camera Control
	void LookMouse(const FInputActionValue& InputValue);
	void LookGamepad(const FInputActionValue& InputValue);

	// Interaction
	void PrimaryInteract();

	// Health and Death
	virtual void OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float DesiredDelta, float ActualDelta) override;

	/*
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
	UInputAction* Input_UltimateAction;*/
};
