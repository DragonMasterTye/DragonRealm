// Copyright Landon Morrison 2024


#include "DR_AttributeComponent.h"

// Sets default values for this component's properties
UDR_AttributeComponent::UDR_AttributeComponent()
{
	CurrentHealth = MaxHealth;
}

bool UDR_AttributeComponent::IsAlive() const
{
	if(CurrentHealth > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UDR_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UDR_AttributeComponent::ApplyHealthChange(float Delta)
{
	CurrentHealth += Delta;

	OnCurrentHealthChanged.Broadcast(nullptr, this, CurrentHealth, Delta);
	
	return true;
}

