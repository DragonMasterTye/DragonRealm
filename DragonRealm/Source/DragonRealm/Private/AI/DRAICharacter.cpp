// Copyright Landon Morrison 2024


#include "AI/DRAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActionSystem/DRActionComponent.h"
#include "ActionSystem/DRAttributeComponent.h"
#include "World/DRWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/DRPlayerCharacter.h"
#include "Player/DRPlayerState.h"

// Ctor
ADRAICharacter::ADRAICharacter()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<UDRAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UDRActionComponent>("ActionComponent");

	TimeOfHitParamName = "DR_TimeOfHit";
	MaxEXP = 50;
	MinEXP = 10;
	EXPToGrant = FMath::RandRange(MinEXP, MaxEXP);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

void ADRAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ADRAICharacter::OnPawnSeen);
	AttributeComponent->OnCurrentHealthChanged.AddDynamic(this, &ADRAICharacter::OnCurrentHealthChanged);
}

void ADRAICharacter::OnCurrentHealthChanged(AActor* InstigatorActor, UDRAttributeComponent* OwningComponent,
	float NewHealth, float Delta, float ActualDelta)
{
	if(ActualDelta < 0.f)
	{
		// Turn on MF_DR_HitFlash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UDRWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		// Died
		if(NewHealth <= 0.0f)
		{
			ADRPlayerCharacter* PlayerCharacter = Cast<ADRPlayerCharacter>(InstigatorActor);
			if(PlayerCharacter)
			{
				ADRPlayerState* PS = Cast<ADRPlayerState>(PlayerCharacter->GetPlayerState());
				if(PS)
				{
					PS->AddExp(EXPToGrant);
				}
			}
			
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);

			// Remove Health Bar
			ActiveHealthBar->RemoveFromParent();

			// Set Collision and disable movement
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			SetLifeSpan(10.0f);
		}
	}
}

void ADRAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

bool ADRAICharacter::IsAlive()
{
	return AttributeComponent->IsAlive();
}

void ADRAICharacter::OnPawnSeen_Implementation(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

