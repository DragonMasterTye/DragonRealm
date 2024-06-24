// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DRPlayerCharacter.h"

#include "Core/DRInteractionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "ActionSystem/Projectiles/DRProjectile.h"
#include "ActionSystem/DRActionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADRPlayerCharacter::ADRPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySpawnSocket = "Magic_R_Socket";
	TimeOfHitParamName = "DR_TimeOfHit";
	
	// Physical(Scene) Components
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	AbilityArrowComponent = CreateDefaultSubobject<UArrowComponent>("AbilityArrowComponent");
	AbilityArrowComponent->SetupAttachment(GetMesh(), AbilitySpawnSocket);

	// Imaginary(Actor) Components
	InteractionComponent = CreateDefaultSubobject<UDRInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UDRActionComponent>("ActionComponent");
	
	// Setting Defaults
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ADRPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADRPlayerCharacter::OnCurrentHealthChanged);
}

FVector ADRPlayerCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

// Called when the game starts or when spawned
void ADRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

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

void ADRPlayerCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ADRPlayerCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ADRPlayerCharacter::PrimaryAttack()
{	
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ADRPlayerCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ADRPlayerCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(PrimaryAttackProjectileClass);
}

void ADRPlayerCharacter::UltAbility()
{
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_UltAbility, this, &ADRPlayerCharacter::UltAbility_TimeElapsed, 0.2f);
}

void ADRPlayerCharacter::UltAbility_TimeElapsed()
{
	SpawnProjectile(UltProjectileClass);
}

void ADRPlayerCharacter::DashAbility()
{
	PlayAnimMontage(AttackMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_DashAbility, this, &ADRPlayerCharacter::DashAbility_TimeElapsed, 0.2f);
}

void ADRPlayerCharacter::DashAbility_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

/*void ADRPlayerCharacter::Turn(float Value)
{
	
}

void ADRPlayerCharacter::LookUp(float Value)
{
}*/

void ADRPlayerCharacter::OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent,
	float NewHealth, float Delta)
{	
	if(Delta < 0.f)
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

			SetLifeSpan(10.f);
		}
	}
	
}

// Called every frame
void ADRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADRPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADRPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ADRPlayerCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("UltAbility", IE_Pressed, this, &ADRPlayerCharacter::UltAbility);
	PlayerInputComponent->BindAction("DashAbility", IE_Pressed, this, &ADRPlayerCharacter::DashAbility);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADRPlayerCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADRPlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADRPlayerCharacter::StopSprint);
}

void ADRPlayerCharacter::DR_HealSelf(float Amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

void ADRPlayerCharacter::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

FRotator ADRPlayerCharacter::CalculateAimRotation()
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

void ADRPlayerCharacter::SpawnProjectile_Implementation(TSubclassOf<ADRProjectile> ProjectileClass)
{
	if(ensureAlways(ProjectileClass))
	{
		FTransform SpawnTransform = FTransform(CalculateAimRotation(),AbilityArrowComponent->GetComponentLocation());

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<ADRProjectile>(ProjectileClass, SpawnTransform, SpawnParams);
	}
}

