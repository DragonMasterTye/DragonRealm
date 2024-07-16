// Copyright Landon Morrison 2024

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DRGameplayFunctionLibrary.generated.h"

UCLASS()
class DRAGONREALM_API UDRGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Damage
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DR|Gameplay")
	static bool DR_ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DR|Gameplay")
	static bool DR_ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);
	
	//Healing
	static bool Dr_ApplyHealing(AActor* Healer, AActor* TargetActor, float HealAmount);
	
	
};
