// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    const auto Owner = OwnerComp.GetAIOwner()->GetPawn();
    if(!Owner) return EBTNodeResult::Failed;

    const auto AttributeComp = Owner->FindComponentByClass<USAttributeComponent>();
    if(!AttributeComp || !AttributeComp->IsAlive()) return EBTNodeResult::Failed;
    
    const auto Result = AttributeComp->ApplyHealthChange(HealAmount);
    return Result ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
