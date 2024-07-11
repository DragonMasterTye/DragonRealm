// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DRBaseCharacter.generated.h"

class UDRAttributeComponent;
class UDRActionComponent;
UCLASS()
class DRAGONREALM_API ADRBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Ctor
	ADRBaseCharacter();

protected:

	// Component Responsible for Attributes such as Health, Mana, and EXP and their replication
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRAttributeComponent* AttributeComponent;

	// Component Responsible for all the use and replication of Actions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DR|Components")
	UDRActionComponent* ActionComponent;
	
};
