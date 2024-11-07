// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DRBaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DRAGONREALM_API UDRBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	// Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "DR|AS|Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDRBaseAttributeSet, MaxHealth);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "DR|AS|Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDRBaseAttributeSet, Health);

	// Energy
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "DR|AS|Attributes|Energy")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UDRBaseAttributeSet, MaxEnergy);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "DR|AS|Attributes|Energy")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UDRBaseAttributeSet, Energy);

	// GroundMovementSpeed
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxGroundMovementSpeed, Category = "DR|AS|Attributes|GroundMovementSpeed")
	FGameplayAttributeData MaxGroundMovementSpeed;
	ATTRIBUTE_ACCESSORS(UDRBaseAttributeSet, MaxGroundMovementSpeed);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GroundMovementSpeed, Category = "DR|AS|Attributes|GroundMovementSpeed")
	FGameplayAttributeData GroundMovementSpeed;
	ATTRIBUTE_ACCESSORS(UDRBaseAttributeSet, GroundMovementSpeed);

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Health
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	// Energy
	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldValue);

	// GroundMovementSpeed
	UFUNCTION()
	virtual void OnRep_MaxGroundMovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_GroundMovementSpeed(const FGameplayAttributeData& OldValue);
};
