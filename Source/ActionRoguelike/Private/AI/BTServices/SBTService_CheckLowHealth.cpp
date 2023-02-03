// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTServices/SBTService_CheckLowHealth.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SAttributeComponent.h"

USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	LowHealthValue = 20.f;
}

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (ensure(AIController) && ensure(Blackboard))
	{
		APawn* AICharacter = AIController->GetPawn();
		if (ensure(AICharacter))
		{
			const USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AICharacter);
			if (ensure(AttributeComponent))
			{
				const bool bNeedHeal = AttributeComponent->GetHealth() < LowHealthValue;
				Blackboard->SetValueAsBool(NeedHealKey.SelectedKeyName, bNeedHeal);
			}
		}
	}
}
