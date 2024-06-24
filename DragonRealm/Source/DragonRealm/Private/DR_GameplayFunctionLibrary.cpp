// Copyright Landon Morrison 2024


#include "DR_GameplayFunctionLibrary.h"

#include "DR_AttributeComponent.h"

bool UDR_GameplayFunctionLibrary::DR_ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UDR_AttributeComponent* AttributeComponent = UDR_AttributeComponent::GetAttributes(TargetActor);
	if(AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UDR_GameplayFunctionLibrary::DR_ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if(DR_ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if(HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
