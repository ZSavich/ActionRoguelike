// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
    Super::BeginPlay();

    ensure(BehaviorTree);
    RunBehaviorTree(BehaviorTree);

    const auto TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(!TargetPawn) return;
    
    GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
}
