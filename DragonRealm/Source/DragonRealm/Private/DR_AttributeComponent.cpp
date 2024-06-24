// Copyright Landon Morrison 2024


#include "DR_AttributeComponent.h"

#include "DR_GameModeBase.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("DR.DamageMultiplier"), 1.0f, TEXT("Global Damage Multiplier for AttributeComponent"), ECVF_Cheat);

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

bool UDR_AttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

void UDR_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UDR_AttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	if(Delta < 0.f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	float ActualDelta = CurrentHealth - OldHealth;
	OnCurrentHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, ActualDelta);

	// Died
	if(ActualDelta < 0.f && CurrentHealth <= 0.f)
	{
		ADR_GameModeBase* GM = GetWorld()->GetAuthGameMode<ADR_GameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

