// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DR_WorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class DRAGONREALM_API UDR_WorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// 
	
	// Properties
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;

protected:

	// Widget Parts
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	// Properties
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector PositionOffset;

	// Unreal Functions
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
