// Copyright Landon Morrison 2024

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

DEFINE_LOG_CATEGORY(LogDRPlayerCharacter);

ADRPlayerCharacter::ADRPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named BP_DRPlayerCharacter (to avoid direct content references in C++)
}

void ADRPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ADRPlayerCharacter::Look);
	}
	else
	{
		UE_LOG(LogDRPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADRPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADRPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/////////////////////////////////////////////
//
// REFACTORING IN PROGRESS
//
/////////////////////////////////////////////
/*
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
	}*

	AddControllerYawInput(Value.X * (LookYawRate /** RateMultiplier*) * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(Value.Y * (LookPitchRate /** RateMultiplier*) * GetWorld()->GetDeltaSeconds());
}

// Utility Functions
/*void ADRPlayerCharacter::DR_HealSelf(float Amount /* = 100 )
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}*

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
