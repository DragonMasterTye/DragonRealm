// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "Core/DRBaseCharacter.h"
#include "DRTargetDummy.generated.h"

class UDRAttributeComponent;

UCLASS()
class DRAGONREALM_API ADRTargetDummy : public ADRBaseCharacter
{
	GENERATED_BODY()
	
public:	
	ADRTargetDummy();

protected:

	// Used for Hit Flash Effect
	UPROPERTY(VisibleAnywhere, Category = "DR|Effects")
	FName TimeOfHitParamName;

	virtual void OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float DesiredDelta, float ActualDelta) override;

};
