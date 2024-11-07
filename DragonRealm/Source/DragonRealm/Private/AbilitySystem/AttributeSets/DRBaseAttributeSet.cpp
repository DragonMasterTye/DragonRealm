// Copyright Landon Morrison 2024


#include "AbilitySystem/AttributeSets/DRBaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UDRBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Validating Health range
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	// @TODO update this to have move speed and MAX move speed
	else if(Data.EvaluatedData.Attribute == GetMaxGroundMovementSpeedAttribute())
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
		UCharacterMovementComponent* CharacterMovementComponent = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
		const float MaxSpeed = GetMaxGroundMovementSpeed();

		if(CharacterMovementComponent)
		{
			CharacterMovementComponent->MaxWalkSpeed = MaxSpeed;
		}
	}
	
}

// Health
void UDRBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDRBaseAttributeSet, MaxHealth, OldValue);
}
void UDRBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDRBaseAttributeSet, Health, OldValue);
}

// Energy
void UDRBaseAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDRBaseAttributeSet, MaxEnergy, OldValue);
}
void UDRBaseAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDRBaseAttributeSet, Energy, OldValue);
}

// GroundMovementSpeed
void UDRBaseAttributeSet::OnRep_MaxGroundMovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDRBaseAttributeSet, MaxGroundMovementSpeed, OldValue);
}
void UDRBaseAttributeSet::OnRep_GroundMovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDRBaseAttributeSet, GroundMovementSpeed, OldValue);
}

void UDRBaseAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Health
	DOREPLIFETIME_CONDITION_NOTIFY(UDRBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDRBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);

	// Energy
	DOREPLIFETIME_CONDITION_NOTIFY(UDRBaseAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDRBaseAttributeSet, Energy, COND_None, REPNOTIFY_Always);

	// GroundMovementSpeed
	DOREPLIFETIME_CONDITION_NOTIFY(UDRBaseAttributeSet, MaxGroundMovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDRBaseAttributeSet, GroundMovementSpeed, COND_None, REPNOTIFY_Always);
}
