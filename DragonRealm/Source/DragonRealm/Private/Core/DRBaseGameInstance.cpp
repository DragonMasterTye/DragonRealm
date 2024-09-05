// Copyright Landon Morrison 2024


#include "Core/DRBaseGameInstance.h"
#include "AbilitySystemGlobals.h"

void UDRBaseGameInstance::Init()
{
	Super::Init();

	// This is automatic in 5.3+
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
