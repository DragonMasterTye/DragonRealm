// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DR_AttributeComponent.generated.h"

class UDR_AttributeComponent; // Forward declare to make the DELEGATE shut up
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCurrentHealthChanged, AActor*, Instigator, UDR_AttributeComponent*, OwningComponent, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDR_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UDR_AttributeComponent();
	
	// Functions
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	static UDR_AttributeComponent* GetAttributes(AActor* FromActor);
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
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 100.f;
	
};
