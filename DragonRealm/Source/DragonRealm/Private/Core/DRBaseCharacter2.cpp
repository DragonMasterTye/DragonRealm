// Copyright Landon Morrison 2024


#include "Core/DRBaseCharacter2.h"

#include "AbilitySystem/Components/DRBaseAbilitySystemComponent.h"
#include "Core/DRBaseCharacterMovementComponent.h"

// CTOR
ADRBaseCharacter2::ADRBaseCharacter2(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UDRBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Convert the CharacterMovementComponent into the DragonRealmMovementComponent
	DRCMC = Cast<UDRBaseCharacterMovementComponent>(GetCharacterMovement());

	// Create the DRAbilitySystemComponent
	DRASC = CreateDefaultSubobject<UDRBaseAbilitySystemComponent>(TEXT("DRAbilitySystemComponent"));
}

UAbilitySystemComponent* ADRBaseCharacter2::GetAbilitySystemComponent() const
{
	return DRASC;
}
