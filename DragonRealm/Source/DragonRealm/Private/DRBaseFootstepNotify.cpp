// Copyright Landon Morrison 2024


#include "DRBaseFootstepNotify.h"

#include "Core/DRBaseCharacter.h"
#include "Core/DRBaseFootstepComponent.h"

void UDRBaseFootstepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp);

	ADRBaseCharacter* Character = MeshComp ? Cast<ADRBaseCharacter>(MeshComp->GetOwner()) : nullptr;
	if(Character)
	{
		if(UDRBaseFootstepComponent* FootstepComponent = Character->GetFootstepComponent())
		{
			FootstepComponent->HandleFootstep(Foot);
		}
	}
}
