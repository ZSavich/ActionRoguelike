// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "SAIController.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"

ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    if(!ensure(PawnSensingComp) || !ensure(AttributeComp)) return;
    
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
    AttributeComp->OnDead.AddDynamic(this, &ASAICharacter::OnDeadHandle);
}

void ASAICharacter::OnDeadHandle(AActor* InstigatorActor, AActor* VictimActor)
{
    // TODO: Death Animation && Disable AI && Destroy 
    SetLifeSpan(5.f);
}

void ASAICharacter::OnPawnSeen(APawn* TargetPawn)
{
    if(!TargetPawn->IsA<ASCharacter>()) return;

    const auto AIController = GetController<ASAIController>();
    if(!AIController) return;

    AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
}
