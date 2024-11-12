// Copyright Landon Morrison 2024


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Core/Data/DRBaseDataTypes.h"
#include "Core/Data/DRBaseCharacterDataAsset.h"
#include "GameFramework/Character.h"
#include "DRBaseCharacter.generated.h"


class UDRBaseFootstepComponent;
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

	UDRBaseFootstepComponent* GetFootstepComponent() const;

protected:
	
	// Unreal Overrides
	virtual void PostInitializeComponents() override;

	// Basic Damage text popup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DR|Assignables")
	TSubclassOf<UUserWidget> DamagePopupClass;
	
	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float DesiredDelta, float ActualDelta);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
// CURRENTLY DISABLED
// CMC -------------------------------------------------------------------------------------------
#pragma region CustomMovementComponent
	/*
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
*/
#pragma endregion 
// CMC -------------------------------------------------------------------------------------------
// CURRENTLY DISABLED

public:
	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, float Level, FGameplayEffectContextHandle EffectContext);

	UFUNCTION(BlueprintCallable)
	FDRCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(const FDRCharacterData& InCharacterData);
	
protected:
	// Custom Ability System Component Responsible for all Attributes and Abilities
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDRBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	UDRBaseAttributeSet* AttributeSet;

	/* DEPRECATED
	// We should not modify attributes directly so we have this set of effects to do it instead
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DR|AS|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeSet;
	
	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DR|AS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Default Effects such as health regen
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DR|AS|Effects")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
	*/
	
	// Initialization functions
	/* DEPRECATED
	void InitializeAttributes();
	*/
	void GiveAbilities();
	void ApplyStartupEffects();

	// Init on Server
	virtual void PossessedBy(AController* NewController) override;
	// Init on Client
	virtual void OnRep_PlayerState() override;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FDRCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();

	virtual void InitFromCharacterData(const FDRCharacterData& InCharacterData, bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly)
	UDRBaseCharacterDataAsset* CharacterDataAsset;

	UPROPERTY(BlueprintReadOnly)
	UDRBaseFootstepComponent* FootstepComponent;
};
