// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "DRTargetDummy.generated.h"

class UDRAttributeComponent;
UCLASS()
class DRAGONREALM_API ADRTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRTargetDummy();

protected:

	// Physical(Scene) Components
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	// Imaginary(Actor) Components
	UPROPERTY(VisibleAnywhere)
	UDRAttributeComponent* AttributeComponent;

	// Properties
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeOfHitParamName;

	// Unreal Functions
	virtual void PostInitializeComponents() override;

	// Functions
	UFUNCTION(BlueprintNativeEvent)
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth, float Delta, float ActualDelta);

};
