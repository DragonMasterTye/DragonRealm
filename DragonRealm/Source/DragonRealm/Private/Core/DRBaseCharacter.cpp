// Copyright Landon Morrison 2024


#include "Core/DRBaseCharacter.h"

#include "AbilitySystem/DRAbilitySystemComponent.h"
#include "ActionSystem/DRActionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "Blueprint/UserWidget.h"
#include "Core/DRCharacterMovementComponent.h"

// Ctor
ADRBaseCharacter::ADRBaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UDRCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{	
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetGenerateOverlapEvents(true);

	DRCharacterMovementComponent = Cast<UDRCharacterMovementComponent>(GetCharacterMovement());
	DRCharacterMovementComponent->SetIsReplicated(true);
	
	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UDRActionComponent>("ActionComponent");
	AbilitySystemComponent = CreateDefaultSubobject<UDRAbilitySystemComponent>("DRAbilitySystemComponent");
}

FCollisionQueryParams ADRBaseCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ADRBaseCharacter::Jump()
{
	bPressedDRJump = true;

	Super::Jump();
	
	bPressedJump = false;

	UE_LOG(LogTemp, Warning, TEXT("Jump isserver:%d"), HasAuthority())
}

void ADRBaseCharacter::StopJumping()
{
	bPressedDRJump = false;
	Super::StopJumping();
}

UAbilitySystemComponent* ADRBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ADRBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//AttributeComponent->OnHealthChanged.AddDynamic(this, &ADRBaseCharacter::OnHealthChanged);
}

void ADRBaseCharacter::OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth,
                                       float DesiredDelta, float ActualDelta)
{
	if(ensureMsgf(DamagePopupClass, TEXT("No DamagePopupClass Assigned")))
	{
		CreateWidget(GetWorld(), DamagePopupClass)->AddToViewport();
	}
}
