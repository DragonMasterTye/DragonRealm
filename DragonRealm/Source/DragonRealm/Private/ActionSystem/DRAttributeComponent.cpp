// Copyright Landon Morrison 2024


#include "ActionSystem/DRAttributeComponent.h"

#include "Core/DRGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("DR.DamageMultiplier"), 1.0f, TEXT("Global Damage Multiplier for AttributeComponent"), ECVF_Cheat);

// Ctor
UDRAttributeComponent::UDRAttributeComponent()
{
	SetIsReplicatedByDefault(true);
	
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

// Functions-----------------------------------------------------
UDRAttributeComponent* UDRAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UDRAttributeComponent>(FromActor->GetComponentByClass(UDRAttributeComponent::StaticClass()));
	}

	return nullptr;
}

// Returns false (DEAD) if FromActor doesn't have an AttributeComponent
bool UDRAttributeComponent::IsActorAlive(AActor* FromActor)
{
	UDRAttributeComponent* AttributeComponent = GetAttributes(FromActor);
	if(AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

bool UDRAttributeComponent::IsAlive() const
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

bool UDRAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

bool UDRAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}

	if(Delta < 0.f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	
	float OldHealth = CurrentHealth;
	float NewHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	if(GetOwner()->HasAuthority() && ActualDelta != 0.f)
	{
		CurrentHealth = NewHealth;
		MulticastCurrentHealthChanged(InstigatorActor, CurrentHealth, Delta, ActualDelta);
		
		// Check if Killed
		if(ActualDelta < 0.f && CurrentHealth <= 0.f)
		{
			ADRGameModeBase* GM = GetWorld()->GetAuthGameMode<ADRGameModeBase>();
			if(GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

// Replication
void UDRAttributeComponent::MulticastCurrentHealthChanged_Implementation(AActor* Instigator, float NewHealth,
	float Delta, float ActualDelta)
{
	OnCurrentHealthChanged.Broadcast(Instigator, this, NewHealth, Delta, ActualDelta);
}

void UDRAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDRAttributeComponent, CurrentHealth);
	DOREPLIFETIME(UDRAttributeComponent, MaxHealth);
}