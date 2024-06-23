// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "DR_AttributeComponent.h"
#include "GameFramework/Actor.h"
#include "DR_TargetDummy.generated.h"

class UDR_AttributeComponent;
UCLASS()
class DRAGONREALM_API ADR_TargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADR_TargetDummy();

protected:

	// Physical Components
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	// Imaginary Components
	UPROPERTY(VisibleAnywhere)
	UDR_AttributeComponent* AttributeComponent;

	// Unreal Functions
	virtual void PostInitializeComponents() override;

	// Functions
	UFUNCTION(BlueprintNativeEvent)
	void OnCurrentHealthChanged(AActor* InstigatorActor, UDR_AttributeComponent* OwningComponent, float NewHealth, float Delta);

};
