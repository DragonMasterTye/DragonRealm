// Copyright Landon Morrison 2024


#include "Core/DRGameplayFunctionLibrary.h"

#include "ActionSystem/DRAttributeComponent.h"

bool UDRGameplayFunctionLibrary::DR_ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UDRAttributeComponent* AttributeComponent = UDRAttributeComponent::GetAttributeComponent(TargetActor);
	if(AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UDRGameplayFunctionLibrary::DR_ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if(DR_ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if(HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitComponent->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

bool UDRGameplayFunctionLibrary::Dr_ApplyHealing(AActor* Healer, AActor* TargetActor, float HealAmount)
{
	UDRAttributeComponent* AttributeComponent = UDRAttributeComponent::GetAttributeComponent(TargetActor);
	if(AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(Healer, HealAmount);
	}
	return false;
}
