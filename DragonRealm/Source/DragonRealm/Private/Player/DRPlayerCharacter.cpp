// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DRPlayerCharacter.h"

#include "Core/DRInteractionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "ActionSystem/Projectiles/DRProjectile.h"
#include "ActionSystem/DRActionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/DRLockOnComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Ctor
ADRPlayerCharacter::ADRPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones; //@TODO update this to only tick when needed
	
	// Physical(Scene) Components
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpawnTransformArrow = CreateDefaultSubobject<UArrowComponent>("SpawnTransformArrow");
	SpawnTransformArrow->SetupAttachment(GetMesh(), AbilitySpawnSocket);

	// Imaginary(Actor) Components
	InteractionComponent = CreateDefaultSubobject<UDRInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UDRActionComponent>("ActionComponent");
	LockOnComponent = CreateDefaultSubobject<UDRLockOnComponent>("LockOnComponent");
	
	// Setting Defaults
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Unreal Functions
void ADRPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADRPlayerCharacter::OnCurrentHealthChanged);
}

void ADRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADRPlayerCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ADRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADRPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADRPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ADRPlayerCharacter::PrimaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ADRPlayerCharacter::SecondaryAction);
	PlayerInputComponent->BindAction("UltimateAction", IE_Pressed, this, &ADRPlayerCharacter::UltimateAction);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADRPlayerCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADRPlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADRPlayerCharacter::StopSprint);
}

// Functions --------------------------------------
void ADRPlayerCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ADRPlayerCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward
	// Y = Right
	// Z = Up
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ADRPlayerCharacter::OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent,
	float NewHealth, float Delta, float ActualDelta)
{	
	if(ActualDelta < 0.f)
	{
		// Turn on MF_DR_HitFlash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);

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
}

void ADRPlayerCharacter::DR_HealSelf(float Amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

UCameraComponent* ADRPlayerCharacter::GetCamera() const
{
	return CameraComponent;
}

void ADRPlayerCharacter::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

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
}
