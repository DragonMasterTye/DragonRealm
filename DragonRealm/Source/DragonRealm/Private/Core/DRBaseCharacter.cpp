// Copyright Landon Morrison 2024


#include "Core/DRBaseCharacter.h"

#include "ActionSystem/DRActionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"

// Ctor
ADRBaseCharacter::ADRBaseCharacter()
{
	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UDRActionComponent>("ActionComponent");
}
