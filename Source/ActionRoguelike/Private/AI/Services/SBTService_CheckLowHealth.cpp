// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto Owner = OwnerComp.GetAIOwner()->GetPawn();
    if(!Owner) return;
    
    const auto AttributeComp = Owner->FindComponentByClass<USAttributeComponent>();
    if(!AttributeComp) return;

    const auto Result = AttributeComp->GetCurrentHealth() < MinHealth;
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(NeedHealKey.SelectedKeyName, Result);
}
