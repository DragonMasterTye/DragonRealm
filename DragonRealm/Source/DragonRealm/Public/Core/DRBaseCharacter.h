// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "DRBaseCharacter.generated.h"

class UDRCharacterMovementComponent;
//class UDRAbilitySystemComponent;
class UDRAttributeComponent;
class UDRActionComponent;
UCLASS()
class DRAGONREALM_API ADRBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Ctor
	ADRBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure)
	FORCEINLINE UDRCharacterMovementComponent* GetDRCharacterMovement() const { return DRCharacterMovementComponent; }

protected:
	// Component Responsible for Attributes such as Health, Mana, and EXP and their replication
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRAttributeComponent* AttributeComponent;
	// Component Responsible for all the use and replication of Actions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRActionComponent* ActionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Movement")
	UDRCharacterMovementComponent* DRCharacterMovementComponent;

	// Unreal Overrides
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Assignables")
	TSubclassOf<UUserWidget> DamagePopupClass;
	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float DesiredDelta, float ActualDelta);

// Ability System Section -------------------------------------------------------------------------------------------
	
public:
	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Custom Ability System Component Responsible for all Attributes and Abilities
	TWeakObjectPtr<class UDRAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UDRBaseAttributeSet> AttributeSetBase;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DR|Abilities")
	TArray<TSubclassOf<class UDRGameplayAbility>> CharacterAbilities;

// Ability System Section -------------------------------------------------------------------------------------------
};
