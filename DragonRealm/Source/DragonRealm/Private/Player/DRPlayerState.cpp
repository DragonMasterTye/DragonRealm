// Copyright Landon Morrison 2024


#include "Player/DRPlayerState.h"

#include "AbilitySystem/Components/DRBaseAbilitySystemComponent.h"
#include "Core/DRBaseSaveGame.h"

ADRPlayerState::ADRPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDRBaseAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ADRPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 ADRPlayerState::GetEXP() const
{
	return EXP;
}

void ADRPlayerState::AddExp(int32 EXPToAdd)
{
	int32 OldEXP = EXP;
	EXP += EXPToAdd;
	OnEXPChanged.Broadcast(this, EXP, EXP - OldEXP);
}

void ADRPlayerState::RemoveExp(int32 EXPToRemove)
{
	int32 OldEXP = EXP;
	EXP -= EXPToRemove;
	OnEXPChanged.Broadcast(this, EXP, EXP - OldEXP);
}

// SaveGame
void ADRPlayerState::SavePlayerState_Implementation(UDRBaseSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->EXP = EXP;
	}
}

void ADRPlayerState::LoadPlayerState_Implementation(UDRBaseSaveGame* SaveObject)
{
	if(SaveObject)
	{
		EXP = SaveObject->EXP;
	}
}
