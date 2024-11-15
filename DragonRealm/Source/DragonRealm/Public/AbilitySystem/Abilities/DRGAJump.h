// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DRBaseGameplayAbility.h"
#include "DRGAJump.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRGAJump : public UDRBaseGameplayAbility
{
	GENERATED_BODY()

	UDRGAJump();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "DR|Effects")
	TSubclassOf<UGameplayEffect> JumpEffect;
};
