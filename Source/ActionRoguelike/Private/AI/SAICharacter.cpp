// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "SPlayerState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Widgets/SWorldUserWidget.h"
#include "Components/SAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SActionComponent.h"

ASAICharacter::ASAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Adjust character's properties
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	BaseEyeHeight = 40.f;
	LifeSpanTime = 3.f;
	TimeToHitParamName = "TimeToHit";
	RewardCredits = 50.f;

	// Change Character's Components
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	// Create a pawn sensing component
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponeng"));
	PawnSensingComponent->SetPeripheralVisionAngle(60);

	// Create an attribute component
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	// Create an action component
	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));
}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::HandleOnSeePawn);
	}

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTargetActor)
{
	AAIController* AIController = GetController<AAIController>();
	if (NewTargetActor && ensure(AIController))
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		if (ensure(BlackboardComponent))
		{
			BlackboardComponent->SetValueAsObject("TargetActor", NewTargetActor);
		}
	}
}

AActor* ASAICharacter::GetTargetActor()
{
	AAIController* AIController = GetController<AAIController>();
	if (ensure(AIController))
	{
		const UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		if (ensure(BlackboardComponent))
		{
			if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor")))
			{
				return TargetActor;
			}
		}
	}
	return nullptr;
}

void ASAICharacter::HandleOnSeePawn(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		if (USWorldUserWidget* TargetSpottedWidget = CreateWidget<USWorldUserWidget>(GetWorld(), TargetSpottedWidgetClass))
		{
			TargetSpottedWidget->SetAttachedActor(this);
			TargetSpottedWidget->AddToViewport();
		}
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	// If the bot was killed, we must to stop the AI Logic and destroy it
	if (FMath::IsNearlyZero(NewHealth))
	{
		// Remove health bar widget
		if (HealthBarWidget)
		{
			HealthBarWidget->RemoveFromParent();
		}

		// Stop a Behavior Tree logic
		const AAIController* AIController = GetController<AAIController>();
		if (ensure(Controller))
		{
			AIController->BrainComponent->StopLogic(TEXT("Death"));
		}

		// Disable collision for capsule component
		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// Disable any movements from the movement component
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->DisableMovement();
		}

		// Turn on the ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

		// Reward a player
		if (const APawn* InstigatorPawn = Cast<APawn>(InstigatorActor))
		{
			if (ASPlayerState* InstigatorState = InstigatorPawn->GetPlayerState<ASPlayerState>())
			{
				InstigatorState->ApplyCreditChange(RewardCredits);
			}
		}
		
		// Destroy the bot
		SetLifeSpan(LifeSpanTime);

		return;
	}

	// Show the shining effect whenever the bot gets damage
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (InstigatorActor && InstigatorActor->IsA(ACharacter::StaticClass()))
		{
			SetTargetActor(InstigatorActor);
		}
	}

	if (!HealthBarWidget)
	{
		HealthBarWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			HealthBarWidget->SetAttachedActor(this);
			HealthBarWidget->AddToViewport();
		}
	}
}
