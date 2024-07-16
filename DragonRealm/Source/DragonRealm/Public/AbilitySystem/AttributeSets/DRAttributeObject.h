// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/NoExportTypes.h"

#include "DRAttributeObject.generated.h"



UCLASS()
class DRAGONREALM_API UDRAttributeObject : public UObject
{
	GENERATED_BODY()

	UDRAttributeObject();

	FName AttributeName;
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "DR|Attribute", ReplicatedUsing = "OnRep_Attribute")
	FGameplayAttributeData AttributeData;

	UFUNCTION()
	virtual void OnRep_Attribute(const FGameplayAttributeData& OldValue);
};
