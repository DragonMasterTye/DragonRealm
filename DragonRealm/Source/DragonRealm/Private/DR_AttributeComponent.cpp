// Copyright Landon Morrison 2024


#include "DR_AttributeComponent.h"

// Sets default values for this component's properties
UDR_AttributeComponent::UDR_AttributeComponent()
{
	CurrentHealth = MaxHealth;
}

void UDR_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UDR_AttributeComponent::ApplyHealthChange(float Delta)
{
	CurrentHealth += Delta;
	
	return true;
}

