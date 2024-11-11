// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DRBaseAnimInstance.generated.h"

class UDRBaseCharacterAnimDataAsset;
class UBlendSpace;
class UDRActionComponent;

UCLASS()
class DRAGONREALM_API UDRBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	/* DEPRECATED
	// Properties
	UPROPERTY(BlueprintReadOnly, Category = "DR|Animation")
	UDRActionComponent* ActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DR|Animation")
	bool bIsStunned;

	// Unreal Functions
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	*/

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UBlendSpace* GetGroundLocomotionBlendSpace() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequenceBase* GetGroundIdleAnimations() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DR|Animation")
	UDRBaseCharacterAnimDataAsset* DefaultCharacterAnimDataAsset;
};
