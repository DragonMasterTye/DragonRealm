// Copyright Landon Morrison 2024


#include "ActionSystem/DRAttributeComponent.h"

#include "Core/DRBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

static TAutoConsoleVariable<float> DebugDamageMultiplier(TEXT("DR.DamageMultiplier"), 1.0f, TEXT("Global Damage Multiplier for AttributeComponent"), ECVF_Cheat);

// Ctor
UDRAttributeComponent::UDRAttributeComponent()
{
	SetIsReplicatedByDefault(true);

	/*
	for(EDR_Attribute Attribute : TEnumRange<EDR_Attribute>())
	{
		// Create a new Attribute with Base and Current values of 0.0f
		AttributeValues[Attribute] = FDRAttribute(); 
	}
	static FProperty* Property = */
}

// Utility Functions 
UDRAttributeComponent* UDRAttributeComponent::GetAttributeComponent(AActor* FromActor)
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
	UDRAttributeComponent* AttributeComponent = GetAttributeComponent(FromActor);
	if(AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

bool UDRAttributeComponent::IsAlive() const
{
	if(Health > 0)
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

bool UDRAttributeComponent::FullHeal(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, MaxHealth);

}

// Negative number for Damage, Positive number for Heal
bool UDRAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	Delta *= DebugDamageMultiplier.GetValueOnGameThread();
	
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	if(GetOwner()->HasAuthority() && ActualDelta != 0.f)
	{
		Health = NewHealth;
		MulticastHealthChanged(InstigatorActor, Health, Delta, ActualDelta);
		
		// Check if Killed
		if(ActualDelta < 0.f && Health <= 0.f)
		{
			ADRBaseGameMode* GM = GetWorld()->GetAuthGameMode<ADRBaseGameMode>();
			if(GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

bool UDRAttributeComponent::ApplyEnergyChange(AActor* InstigatorActor, float Delta)
{
	float OldEnergy = Energy;
	float NewEnergy = FMath::Clamp(Energy + Delta, 0.0f, MaxEnergy);
	float ActualDelta = NewEnergy - OldEnergy;

	if(GetOwner()->HasAuthority() && ActualDelta != 0.f)
	{
		Energy = NewEnergy;
		MulticastEnergyChanged(InstigatorActor, Energy, Delta, ActualDelta);
	}
	
	return ActualDelta != 0;
}

bool UDRAttributeComponent::ApplyAttributeChange(AActor* InstigatorActor, float Delta, EDR_Attribute AttributeToChange)
{
	if(Delta != 0.0f)
	{
		return false;
	}

	return false;
}

// Replication of Attributes
// Health
void UDRAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth,
																	float DesiredDelta, float ActualDelta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, DesiredDelta, ActualDelta);
}
void UDRAttributeComponent::MulticastMaxHealthChanged_Implementation(AActor* Instigator, float NewMaxHealth,
																	 float DesiredDelta, float ActualDelta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewMaxHealth, DesiredDelta, ActualDelta);
}

// Energy
void UDRAttributeComponent::MulticastEnergyChanged_Implementation(AActor* Instigator, float NewEnergy,
																  float DesiredDelta, float ActualDelta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewEnergy, DesiredDelta, ActualDelta);
}
void UDRAttributeComponent::MulticastMaxEnergyChanged_Implementation(AActor* Instigator, float NewMaxEnergy,
																	float DesiredDelta, float ActualDelta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewMaxEnergy, DesiredDelta, ActualDelta);
}

// Unreal Function to mark replicated properties
void UDRAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDRAttributeComponent, Health);
	DOREPLIFETIME(UDRAttributeComponent, MaxHealth);
	DOREPLIFETIME(UDRAttributeComponent, Energy);
	DOREPLIFETIME(UDRAttributeComponent, MaxEnergy);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	
	//DOREPLIFETIME_WITH_PARAMS_FAST(UDRAttributeComponent, Attributes, Params);
}