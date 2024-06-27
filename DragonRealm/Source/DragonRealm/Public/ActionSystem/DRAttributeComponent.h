// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRAttributeComponent.generated.h"

class UDRAttributeComponent; // Forward declare to make the DELEGATE shut up
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnCurrentHealthChanged, AActor*, Instigator, UDRAttributeComponent*, OwningComponent, float, NewHealth, float, Delta, float, ActualDelta);

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
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnCurrentHealthChanged OnCurrentHealthChanged;
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

protected:

	// Properties
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attributes")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;

	// Replication
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCurrentHealthChanged(AActor* Instigator, float NewHealth, float Delta, float ActualDelta);
	
};
