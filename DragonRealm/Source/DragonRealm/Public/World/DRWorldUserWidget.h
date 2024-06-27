// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DRWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class DRAGONREALM_API UDRWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// 
	
	// Properties
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta =(ExposeOnSpawn=true))
	AActor* AttachedActor;

protected:

	// Widget Parts
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector WorlPositionOffset;

	// Unreal Functions
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
