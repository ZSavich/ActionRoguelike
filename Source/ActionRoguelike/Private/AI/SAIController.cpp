// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
    Super::BeginPlay();

    if(!ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please select it in the Player Controller"))) return;
    RunBehaviorTree(BehaviorTree);

    
    /*
     * Bad way to find Target Actor
    const auto TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(!TargetPawn) return;
    GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
    */
}
