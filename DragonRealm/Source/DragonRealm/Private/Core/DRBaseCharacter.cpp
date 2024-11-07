// Copyright Landon Morrison 2024


#include "Core/DRBaseCharacter.h"

#include "AbilitySystem/Abilities/DRBaseGameplayAbility.h"
#include "AbilitySystem/AttributeSets/DRBaseAttributeSet.h"
#include "AbilitySystem/Components/DRBaseAbilitySystemComponent.h"
#include "ActionSystem/DRActionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "Blueprint/UserWidget.h"
#include "Core/DRBaseCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Ctor
ADRBaseCharacter::ADRBaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UDRBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{	
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetGenerateOverlapEvents(true);

	//DRBaseCharacterMovementComponent = Cast<UDRBaseCharacterMovementComponent>(GetCharacterMovement());
	//DRBaseCharacterMovementComponent->SetIsReplicated(true);
	
	AbilitySystemComponent = CreateDefaultSubobject<UDRBaseAbilitySystemComponent>(TEXT("DRBaseAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UDRBaseAttributeSet>(TEXT("DRBaseAttributeSet"));
}

FCollisionQueryParams ADRBaseCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ADRBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}

void ADRBaseCharacter::OnHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent, float NewHealth,
									   float DesiredDelta, float ActualDelta)
{
	if(ensureMsgf(DamagePopupClass, TEXT("No DamagePopupClass Assigned")))
	{
		CreateWidget(GetWorld(), DamagePopupClass)->AddToViewport();
	}
}

void ADRBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADRBaseCharacter, CharacterData);
}


// CURRENTLY DISABLED
// CMC -------------------------------------------------------------------------------------------
#pragma region CustomCharacterMovement
/*
void ADRBaseCharacter::Jump()
{
	bPressedDRJump = true;

	Super::Jump();
	
	bPressedJump = false;

	//UE_LOG(LogTemp, Warning, TEXT("Jump isserver:%d"), HasAuthority())
}

void ADRBaseCharacter::StopJumping()
{
	bPressedDRJump = false;
	Super::StopJumping();
}*/
#pragma endregion
// CMC -------------------------------------------------------------------------------------------
// CURRENTLY DISABLED

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AbilitySystem -------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region AbilitySystem
UAbilitySystemComponent* ADRBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADRBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	/* DEPRECATED
	InitializeAttributes();
	*/
	GiveAbilities();
	ApplyStartupEffects();
}

void ADRBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	/* DEPRECATED
	InitializeAttributes();
	*/
}

/* DEPRECATED
void ADRBaseCharacter::InitializeAttributes()
{
	if(HasAuthority() && DefaultAttributeSet && AttributeSet)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		
		ApplyGameplayEffectToSelf(DefaultAttributeSet, 1, EffectContextHandle);
	}
}
*/

void ADRBaseCharacter::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for(auto DefaultAbility : CharacterData.Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void ADRBaseCharacter::ApplyStartupEffects()
{
	if(HasAuthority())
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		for(auto CharacterEffect : CharacterData.Effects)
		{
			ApplyGameplayEffectToSelf(CharacterEffect, 1, EffectContextHandle);
		}
	}
}

bool ADRBaseCharacter::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, float Level,
	FGameplayEffectContextHandle EffectContext)
{
	if(!Effect.Get())
	{
		return false;
	}

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, Level, EffectContext);
	if(SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}
	
	return false;
}

FDRCharacterData ADRBaseCharacter::GetCharacterData() const
{
	return CharacterData;
}

void ADRBaseCharacter::SetCharacterData(const FDRCharacterData& InCharacterData)
{
	CharacterData = InCharacterData;

	InitFromCharacterData(CharacterData);
}

void ADRBaseCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void ADRBaseCharacter::InitFromCharacterData(const FDRCharacterData& InCharacterData, bool bFromReplication)
{
	
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AbilitySystem -------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////