// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Core/Data/DRBaseDataTypes.h"
#include "DRBaseFootstepNotify.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONREALM_API UDRBaseFootstepNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	EFoot Foot;
	
};
