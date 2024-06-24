// Copyright Landon Morrison 2024


#include "DR_AttributeComponent.h"

// Sets default values for this component's properties
UDR_AttributeComponent::UDR_AttributeComponent()
{
	CurrentHealth = MaxHealth;
}

UDR_AttributeComponent* UDR_AttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UDR_AttributeComponent>(FromActor->GetComponentByClass(UDR_AttributeComponent::StaticClass()));
	}

	return nullptr;
}

// Returns false (DEAD) if FromActor doesn't have an AttributeComponent
bool UDR_AttributeComponent::IsActorAlive(AActor* FromActor)
{
	UDR_AttributeComponent* AttributeComponent = GetAttributes(FromActor);
	if(AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
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

bool UDR_AttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	float ActualDelta = CurrentHealth - OldHealth;

	OnCurrentHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, ActualDelta);
	
	return ActualDelta != 0;
}

