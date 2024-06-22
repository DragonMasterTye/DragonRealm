// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonRealm/Public/DR_Character.h"

#include "DR_InteractionComponent.h"
#include "DR_AttributeComponent.h"
#include "DR_MagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADR_Character::ADR_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Physical Attributes
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	AbilityArrowComponent = CreateDefaultSubobject<UArrowComponent>("AbilityArrowComponent");
	AbilityArrowComponent->SetupAttachment(GetMesh(), AbilitySpawnSocket);

	// Imaginary Components
	InteractionComponent = CreateDefaultSubobject<UDR_InteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UDR_AttributeComponent>("AttributeComponent");
	
	// Setting Defaults
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ADR_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADR_Character::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ADR_Character::MoveRight(float Value)
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

void ADR_Character::PrimaryAttack()
{	
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ADR_Character::PrimaryAttack_TimeElapsed, 0.2f);
}

void ADR_Character::PrimaryAttack_TimeElapsed()
{
	FTransform SpawnTransform = FTransform(CalculateAimRotation(),AbilityArrowComponent->GetComponentLocation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	if(ensureAlways(PrimaryAttackProjectileClass))
	{
		GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectileClass, SpawnTransform, SpawnParams);
	}
}

void ADR_Character::UltAbility()
{
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_UltAbility, this, &ADR_Character::UltAbility_TimeElapsed, 0.2f);
}

void ADR_Character::UltAbility_TimeElapsed()
{
	FTransform SpawnTransform = FTransform(CalculateAimRotation(),AbilityArrowComponent->GetComponentLocation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	if(ensureAlways(UltProjectileClass))
	{
		GetWorld()->SpawnActor<AActor>(UltProjectileClass, SpawnTransform, SpawnParams);
	}
}

/*void ADR_Character::Turn(float Value)
{
	
}

void ADR_Character::LookUp(float Value)
{
}*/

// Called every frame
void ADR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADR_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADR_Character::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ADR_Character::PrimaryAttack);
	PlayerInputComponent->BindAction("UltAbility", IE_Pressed, this, &ADR_Character::UltAbility);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADR_Character::PrimaryInteract);
}

void ADR_Character::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

FRotator ADR_Character::CalculateAimRotation()
{
	FRotator ReturnValue;
	
	FVector SpawnLocation = AbilityArrowComponent->GetComponentLocation();
	FRotator SpawnRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation();

	FHitResult Hit;
	FVector Start = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector End = SpawnLocation + (SpawnRotation.Vector() * 10000.f);

	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	if(bBlockingHit)
	{
		ReturnValue = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, Hit.ImpactPoint);
	}
	else
	{
		ReturnValue = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, End);
	}
	
	return ReturnValue;
}

