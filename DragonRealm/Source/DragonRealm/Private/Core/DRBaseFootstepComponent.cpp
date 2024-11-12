// Copyright Landon Morrison 2024


#include "Core/DRBaseFootstepComponent.h"

#include "Core/DRBaseCharacter.h"
#include "Core/DRBasePhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	0,
	TEXT("Draws Debug Info for FootSteps\n")
	TEXT("0 is OFF, anything above 0 is ON and sets the time to show for"),
	ECVF_Cheat
	);

// Sets default values for this component's properties
UDRBaseFootstepComponent::UDRBaseFootstepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	FootstepVolume = 1.f;

	// ...
}

void UDRBaseFootstepComponent::HandleFootstep(EFoot Foot)
{
	ADRBaseCharacter* Character = Cast<ADRBaseCharacter>(GetOwner());
	if(Character)
	{
		const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();
		
		USkeletalMeshComponent* Mesh = Character->GetMesh();
		if(Mesh)
		{
			FHitResult HitResult;
			const FVector SocketLocation = Mesh->GetSocketLocation(Foot == EFoot::Left ? LeftFootSocketName : RightFootSocketName);
			const FVector Location = SocketLocation + FVector::UpVector * 20;

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;
			QueryParams.AddIgnoredActor(Character);

			if(GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location * -50.f, ECC_WorldStatic, QueryParams))
			{
				if(HitResult.bBlockingHit)
				{
					if(HitResult.PhysMaterial.Get())
					{
						UDRBasePhysicalMaterial* PhysicalMaterial = Cast<UDRBasePhysicalMaterial>(HitResult.PhysMaterial.Get());
						if(PhysicalMaterial)
						{
							UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->FootstepSound, Location, FootstepVolume);
						}

						if(DebugShowFootsteps > 0)
						{
							DrawDebugString(GetWorld(), Location, GetNameSafe(PhysicalMaterial), nullptr, FColor::White, DebugShowFootsteps);
						}
					}

					if(DebugShowFootsteps > 0)
					{
						DrawDebugSphere(GetWorld(), Location, 16, 16, FColor::Red, false, DebugShowFootsteps);
					}
				}
				else
				{
					if(DebugShowFootsteps > 0)
					{
						DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Red, false, DebugShowFootsteps, 0, 1);
					}
				}
			}
			else
			{
				if(DebugShowFootsteps > 0)
				{
					DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Red, false, DebugShowFootsteps, 0, 1);
					DrawDebugSphere(GetWorld(), Location, 16, 16, FColor::Red, false, DebugShowFootsteps);
				}
			}
		}
	}
}


// Called when the game starts
void UDRBaseFootstepComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}