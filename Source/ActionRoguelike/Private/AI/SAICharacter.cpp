// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "SAIController.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* TargetPawn)
{
    if(!TargetPawn->IsA<ASCharacter>()) return;

    const auto AIController = GetController<ASAIController>();
    if(!AIController) return;

    AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
}
