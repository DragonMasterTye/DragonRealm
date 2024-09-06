// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DRPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/DRBaseInteractionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "ActionSystem/Projectiles/DRProjectile.h"
#include "ActionSystem/DRActionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/DRBaseCharacterMovementComponent.h"
#include "Core/DRBaseLockOnComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Ctor
ADRPlayerCharacter::ADRPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones; //@TODO update this to only tick when needed
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Imaginary(Actor) Components
	InteractionComponent = CreateDefaultSubobject<UDRBaseInteractionComponent>("InteractionComponent");
	LockOnComponent = CreateDefaultSubobject<UDRBaseLockOnComponent>("LockOnComponent");
	
	// Setting Defaults
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ADRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADRPlayerCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

// Input
void ADRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultInputMapping, 0);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Move, Jump, Interact
	EnhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::Move);
	EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Started, this, &ADRBaseCharacter::Jump);
	EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Completed, this, &ADRBaseCharacter::StopJumping);
	EnhancedInputComponent->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::PrimaryInteract);

	// Sprint
	//EnhancedInputComponent->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ADRPlayerCharacter::StartSprint);
	//EnhancedInputComponent->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ADRPlayerCharacter::StopSprint);

	// Mouse & Keyboard
	EnhancedInputComponent->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::LookMouse);
	// Gamepad
	EnhancedInputComponent->BindAction(Input_LookGamepad, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::LookGamepad);

	// Actions
	//EnhancedInputComponent->BindAction(Input_PrimaryAction, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::PrimaryAction);
	//EnhancedInputComponent->BindAction(Input_SecondaryAction, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::SecondaryAction);
	//EnhancedInputComponent->BindAction(Input_UltimateAction, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::UltimateAction);
	
}

// Movement
void ADRPlayerCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Get value from input (combined value from WASD keys or single Gamepad stick) and convert to 2D Vector
	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	// Move forward/back
	if(AxisValue.Y != 0.0f)
	{
		AddMovementInput(ControlRot.Vector(), AxisValue.Y);
	}

	// Move Right/Left strafe
	if(AxisValue.X != 0.0f)
	{
		const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
		AddMovementInput(RightVector, AxisValue.X);
	}
}

void ADRPlayerCharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ADRPlayerCharacter::LookGamepad(const FInputActionValue& InputValue)
{
	FVector2D Value = InputValue.Get<FVector2D>();

	// Track negative as we'll lose this during the conversion
	bool XNegative = Value.X < 0.f;
	bool YNegative = Value.Y < 0.f;

	// Can further modify with 'sensitivity' settings
	static const float LookYawRate = 100.0f; // Left Right Secnsitivty
	static const float LookPitchRate = 50.0f; // Up Down Sensitivty

	// non-linear to make aiming a little easier
	Value = Value * Value;

	if (XNegative)
	{
		Value.X *= -1.f;
	}
	if (YNegative)
	{
		Value.Y *= -1.f;
	}

	// Aim assist
	// todo: may need to ease this out and/or change strength based on distance to target
	/*float RateMultiplier = 1.0f;
	if (bHasPawnTarget)
	{
		RateMultiplier = 0.5f;
	}*/

	AddControllerYawInput(Value.X * (LookYawRate /** RateMultiplier*/) * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(Value.Y * (LookPitchRate /** RateMultiplier*/) * GetWorld()->GetDeltaSeconds());
}

// Utility Functions
/*void ADRPlayerCharacter::DR_HealSelf(float Amount /* = 100 )
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}*/

// Interaction
void ADRPlayerCharacter::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

// Health and Death
void ADRPlayerCharacter::OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent,
	float NewHealth, float DesiredDelta, float ActualDelta)
{
	Super::OnHealthChanged(InstigatorActor, OwningComponent, NewHealth, DesiredDelta, ActualDelta);

	// Died
	if(NewHealth <= 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();

		SetLifeSpan(5.f);
	}
}

/*
// Actions
void ADRPlayerCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ADRPlayerCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ADRPlayerCharacter::PrimaryAction()
{	
	ActionComponent->StartActionByName(this, "PrimaryAction");
}

void ADRPlayerCharacter::SecondaryAction()
{
	ActionComponent->StartActionByName(this, "SecondaryAction");
}

void ADRPlayerCharacter::UltimateAction()
{
	ActionComponent->StartActionByName(this, "UltimateAction");
}*/
