// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Run behavior tree for bots
	if (ensureMsgf(BehaviorTree, TEXT("EUD::BehaviorTree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}