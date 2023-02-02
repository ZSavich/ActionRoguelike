// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Adjust character's properties
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	BaseEyeHeight = 40.f;
	
	// Create a pawn sensing component
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponeng"));
	PawnSensingComponent->SetPeripheralVisionAngle(60);

	// Create an attribute component
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::HandleOnSeePawn);
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
	}
}
