// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "DRBaseCharacter2.generated.h"

class UDRBaseAbilitySystemComponent;
class UDRBaseCharacterMovementComponent;
UCLASS()
class DRAGONREALM_API ADRBaseCharacter2 : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	// CTOR
	ADRBaseCharacter2(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:

	// DragonRealmCharacterMovementComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "DR|Movement")
	TObjectPtr<UDRBaseCharacterMovementComponent> DRCMC;
	// DragonRealmAbilitySystemComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "DR|Abilities")
	TObjectPtr<UDRBaseAbilitySystemComponent> DRASC;

	
};
