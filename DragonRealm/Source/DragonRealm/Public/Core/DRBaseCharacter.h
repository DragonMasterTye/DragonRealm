// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "DRBaseCharacter.generated.h"


struct FGameplayEffectContextHandle;
class UGameplayEffect;
class UGameplayAbility;
class UDRBaseCharacterMovementComponent;
class UDRBaseAbilitySystemComponent;
class UDRBaseAttributeSet;
class UDRBaseGameplayAbility;
class UDRAttributeComponent;
class UDRActionComponent;
UCLASS()
class DRAGONREALM_API ADRBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Ctor
	ADRBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	FCollisionQueryParams GetIgnoreCharacterParams() const;

protected:
	/*
	 * Built from scratch personal replacement for GAS
	 * I have decided to use GAS for now so this is disabled
	// Component Responsible for Attributes such as Health, Mana, and EXP and their replication
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRAttributeComponent* AttributeComponent;
	// Component Responsible for all the use and replication of Actions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRActionComponent* ActionComponent;
	*/
	
	// Unreal Overrides
	virtual void PostInitializeComponents() override;

	// Basic Damage text popup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Assignables")
	TSubclassOf<UUserWidget> DamagePopupClass;
	
	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float DesiredDelta, float ActualDelta);

// CMC -------------------------------------------------------------------------------------------
#pragma region CustomMovementComponent
public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE UDRBaseCharacterMovementComponent* GetDRCharacterMovement() const { return DRBaseCharacterMovementComponent; }

	bool bPressedDRJump;
	virtual void Jump() override;
	virtual void StopJumping() override;
	
protected:
	// Custom movement component for alt movement modes like flying, sliding, and wall running
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Movement")
	UDRBaseCharacterMovementComponent* DRBaseCharacterMovementComponent;

#pragma endregion 
// CMC -------------------------------------------------------------------------------------------

// AbilitySystem -------------------------------------------------------------------------------------------
#pragma region AbilitySystem
public:
	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, float Level, FGameplayEffectContextHandle EffectContext);
	
protected:
	// Custom Ability System Component Responsible for all Attributes and Abilities
	UPROPERTY(EditDefaultsOnly)
	UDRBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	UDRBaseAttributeSet* AttributeSet;

	// We should not modify attributes directly so we have this set of effects to do it instead
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DR|AS|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeSet;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DR|AS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Default Effects such as health regen
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DR|AS|Effects")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	// Initialization functions
	void InitializeAttributes();
	void GiveAbilities();
	void ApplyStartupEffects();

	// Init on Server
	virtual void PossessedBy(AController* NewController) override;
	// Init on Client
	virtual void OnRep_PlayerState() override;

#pragma endregion
// AbilitySystem -------------------------------------------------------------------------------------------
};
