// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    if(!TargetActor) return;

    const auto AIController = OwnerComp.GetAIOwner();
    if(!ensure(AIController)) return;

    const auto AIPawn = AIController->GetPawn();
    if(!ensure(AIPawn)) return;

    const auto Distance = FVector(TargetActor->GetActorLocation() - AIPawn->GetActorLocation()).Size();
    const auto bInRange = Distance < AttackRange;
    auto bInLOS = false;
    
    if(bInRange)
    {
        bInLOS = AIController->LineOfSightTo(TargetActor);
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(WithinAttackRange.SelectedKeyName, bInRange && bInLOS);
}
