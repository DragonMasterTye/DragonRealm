// Copyright Landon Morrison 2024


#include "Core/DRBaseCharacter.h"

#include "AbilitySystem/DRAbilitySystemComponent.h"
#include "ActionSystem/DRActionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "Blueprint/UserWidget.h"

// Ctor
ADRBaseCharacter::ADRBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetGenerateOverlapEvents(true);
	
	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UDRActionComponent>("ActionComponent");
	AbilitySystemComponent = CreateDefaultSubobject<UDRAbilitySystemComponent>("DRAbilitySystemComponent");
}

UAbilitySystemComponent* ADRBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ADRBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//AttributeComponent->OnHealthChanged.AddDynamic(this, &ADRBaseCharacter::OnHealthChanged);
}

void ADRBaseCharacter::OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth,
                                       float DesiredDelta, float ActualDelta)
{
	if(ensureMsgf(DamagePopupClass, TEXT("No DamagePopupClass Assigned")))
	{
		CreateWidget(GetWorld(), DamagePopupClass)->AddToViewport();
	}
}
