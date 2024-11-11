// Copyright Landon Morrison 2024


#include "Core/DRBaseAnimInstance.h"

#include "GameplayTagContainer.h"
#include "ActionSystem/DRActionComponent.h"
#include "Core/DRBaseCharacter.h"
#include "Core/Data/DRBaseCharacterAnimDataAsset.h"

/* DEPRECATED
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
*/

UBlendSpace* UDRBaseAnimInstance::GetGroundLocomotionBlendSpace() const
{
	ADRBaseCharacter* Character = Cast<ADRBaseCharacter>(GetOwningActor());
	if(Character)
	{
		FDRCharacterData CharacterData = Character->GetCharacterData();

		if(CharacterData.CharacterAnimDataAsset)
		{
			return CharacterData.CharacterAnimDataAsset->CharacterAnimationData.GroundMovementBlendSpace;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.GroundMovementBlendSpace : nullptr;
}

UAnimSequenceBase* UDRBaseAnimInstance::GetGroundIdleAnimations() const
{
	ADRBaseCharacter* Character = Cast<ADRBaseCharacter>(GetOwningActor());
	if(Character)
	{
		FDRCharacterData CharacterData = Character->GetCharacterData();

		if(CharacterData.CharacterAnimDataAsset)
		{
			return CharacterData.CharacterAnimDataAsset->CharacterAnimationData.GroundIdleAnimation;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.GroundIdleAnimation : nullptr;
}
