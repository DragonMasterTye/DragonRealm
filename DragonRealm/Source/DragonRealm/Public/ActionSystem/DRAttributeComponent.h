// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRAttributeComponent.generated.h"

class UDRAttributeComponent;
// Delegate for changing Attribute values, DesiredDelta and ActualDelta provided for things like damage numbers during an overkill attack or an invulnerable state
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAttributeChanged, AActor*, Instigator, UDRAttributeComponent*, OwningComponent, float, NewValue, float, DesiredDelta, float, ActualDelta);

UENUM()
enum EDR_Attribute
{
	EDRA_Health,
	EDRA_MaxHealth,
	EDRA_Energy,
	EDRA_MaxEnergy
};

USTRUCT()
struct FDRAttribute
{
	GENERATED_BODY()

	FDRAttribute() : CurrentValue(0.0f), BaseValue(0.0f){}
	FDRAttribute(float DefaultValue) : CurrentValue(DefaultValue), BaseValue(DefaultValue){}

	float Value() const { return CurrentValue; }
	float Base() const { return BaseValue; }
	void SetValue(float Value) { CurrentValue = Value; }
	void SetBase(float Base) { BaseValue = Base; }
	
protected:
	float CurrentValue;
	float BaseValue;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Ctor
	UDRAttributeComponent();
	
	// Utility Functions
	UFUNCTION(BlueprintCallable, Category = "DR_Attributes")
	static UDRAttributeComponent* GetAttributeComponent(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "DR|Attributes")
	static bool IsActorAlive(AActor* FromActor);
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	UFUNCTION(BlueprintCallable)
	bool FullHeal(AActor* InstigatorActor);

	// Apply changes to Attributes
	UFUNCTION(Category = "DR|Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DR|Attributes")
	bool ApplyEnergyChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DR|Attributes")
	bool ApplyAttributeChange(AActor* InstigatorActor, float Delta, EDR_Attribute AttributeToChange);


	
	// Attribute Delegates
	UPROPERTY(BlueprintAssignable, Category = "DR|Attributes")
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "DR|Attributes")
	FOnAttributeChanged OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "DR|Attributes")
	FOnAttributeChanged OnEnergyChanged;
	UPROPERTY(BlueprintAssignable, Category = "DR|Attributes")
	FOnAttributeChanged OnMaxEnergyChanged;

protected:

	// Static Array of Attributes that matches the Length of the EDR_Attribute Enum above
	/* UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "DR|Attributes")
	FDRAttribute AttributeValues[TEnumRange<EDR_Attribute>()];*/

	// Attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "DR|Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "DR|Attributes")
	float MaxHealth;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "DR|Attributes")
	float Energy;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "DR|Attributes")
	float MaxEnergy;

	// Replication
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* Instigator, float NewHealth, float DesiredDelta, float ActualDelta);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastMaxHealthChanged(AActor* Instigator, float NewMaxHealth, float DesiredDelta, float ActualDelta);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEnergyChanged(AActor* Instigator, float NewEnergy, float DesiredDelta, float ActualDelta);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastMaxEnergyChanged(AActor* Instigator, float NewMaxEnergy, float DesiredDelta, float ActualDelta);
	
};
