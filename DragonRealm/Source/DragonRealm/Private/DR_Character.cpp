// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonRealm/Public/DR_Character.h"

#include "DR_InteractionComponent.h"
#include "DR_AttributeComponent.h"
#include "DR_Projectile_Base.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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

void ADR_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADR_Character::OnCurrentHealthChanged);
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
	SpawnProjectile(PrimaryAttackProjectileClass);
}

void ADR_Character::UltAbility()
{
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_UltAbility, this, &ADR_Character::UltAbility_TimeElapsed, 0.2f);
}

void ADR_Character::UltAbility_TimeElapsed()
{
	SpawnProjectile(UltProjectileClass);
}

void ADR_Character::DashAbility()
{
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_DashAbility, this, &ADR_Character::DashAbility_TimeElapsed, 0.2f);
}

void ADR_Character::DashAbility_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

/*void ADR_Character::Turn(float Value)
{
	
}

void ADR_Character::LookUp(float Value)
{
}*/

void ADR_Character::OnCurrentHealthChanged(AActor* InstigatorActor, UDR_AttributeComponent* OwningComponent,
	float NewHealth, float Delta)
{
	if(NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
	
	if(Delta < 0.f)
	{
		// Turn on MF_DR_HitFlash
		GetMesh()->SetScalarParameterValueOnMaterials("DR_TimeToHitFlash", GetWorld()->TimeSeconds);
	}
	
}

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
	PlayerInputComponent->BindAction("DashAbility", IE_Pressed, this, &ADR_Character::DashAbility);
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

	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	// Ignore Player
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	
	FVector TraceStart = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.f);
	
	FHitResult Hit;

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 3.f);
	
	if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, Shape, QueryParams))
	{
		TraceEnd = Hit.ImpactPoint;		
	}

	FVector SpawnLocation = AbilityArrowComponent->GetComponentLocation();

	ReturnValue = FRotationMatrix::MakeFromX(TraceEnd - SpawnLocation).Rotator();

	//DrawDebugLine(GetWorld(), SpawnLocation, TraceEnd, FColor::Red, false, 3.f);

	return ReturnValue;
}

void ADR_Character::SpawnProjectile_Implementation(TSubclassOf<ADR_Projectile_Base> ProjectileClass)
{
	if(ensureAlways(ProjectileClass))
	{
		FTransform SpawnTransform = FTransform(CalculateAimRotation(),AbilityArrowComponent->GetComponentLocation());

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<ADR_Projectile_Base>(ProjectileClass, SpawnTransform, SpawnParams);
	}
}

