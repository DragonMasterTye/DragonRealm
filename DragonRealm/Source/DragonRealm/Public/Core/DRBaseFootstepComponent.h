// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DRBaseDataTypes.h"
#include "DRBaseFootstepComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONREALM_API UDRBaseFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDRBaseFootstepComponent();
	
	void HandleFootstep(EFoot Foot);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FName LeftFootSocketName = TEXT("LeftFootSocket");
	UPROPERTY(EditDefaultsOnly)
	FName RightFootSocketName = TEXT("RightFootSocket");

	UPROPERTY(EditAnywhere, Category = "DR|Sounds")
	float FootstepVolume;
};
