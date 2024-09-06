// Copyright Landon Morrison 2024


#include "Core/DRBaseAnimInstance.h"

#include "GameplayTagContainer.h"
#include "ActionSystem/DRActionComponent.h"

// Unreal Functions
void UDRBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if(OwningActor)
	{
		ActionComponent = Cast<UDRActionComponent>(OwningActor->GetComponentByClass(UDRActionComponent::StaticClass()));
	}
}

void UDRBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if(ActionComponent)
	{
		bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
