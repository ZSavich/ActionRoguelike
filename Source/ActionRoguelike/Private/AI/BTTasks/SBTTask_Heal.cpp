// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTasks/SBTTask_Heal.h"

#include "AIController.h"
#include "Components/SAttributeComponent.h"

USBTTask_Heal::USBTTask_Heal()
{
	HealCount = 100.f;
}

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		APawn* AICharacter = AIController->GetPawn();
		if (ensure(AICharacter))
		{
			USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AICharacter);
			if (ensure(AttributeComponent))
			{
				if (AttributeComponent->ApplyHealthChange(AICharacter, HealCount))
				{
					return Super::ExecuteTask(OwnerComp, NodeMemory);
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
