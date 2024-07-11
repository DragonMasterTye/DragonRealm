// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRAttributeComponent.generated.h"

class UDRAttributeComponent;
// Delegate for changing Attribute values, DesiredDelta and ActualDelta provided for things like damage numbers during an overkill attack or an invulnerable state
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAttributeChanged, AActor*, Instigator, UDRAttributeComponent*, OwningComponent, float, NewValue, float, DesiredDelta, float, ActualDelta);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Ctor
	UDRAttributeComponent();
	
	// Functions
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	static UDRAttributeComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	static bool IsActorAlive(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "DR|Attributes")
	bool ApplyDamage(AActor* InstigatorActor, float Delta);
	UPROPERTY(BlueprintAssignable, Category = "DR|Attributes")
	FOnAttributeChanged OnHealthChanged;
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

protected:

	// Properties
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;

	// Replication
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCurrentHealthChanged(AActor* Instigator, float NewHealth, float Delta, float ActualDelta);
	
};
