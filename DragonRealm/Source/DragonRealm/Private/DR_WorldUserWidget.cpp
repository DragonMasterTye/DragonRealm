// Copyright Landon Morrison 2024


#include "DR_WorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UDR_WorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}
	
	FVector2d ScreenPosition;
	FVector Origin;
	FVector BoxExtent;
	AttachedActor->GetActorBounds(true, Origin, BoxExtent, true);
	Origin.Z += BoxExtent.Z;
	FVector WorldPosition = Origin; 
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), WorldPosition + PositionOffset, ScreenPosition))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
