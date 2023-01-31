// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTServices/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckAttackRange::USBTService_CheckAttackRange()
{
	MaxAttackRange = 2000.f;
}

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		const AAIController* BotController = OwnerComp.GetAIOwner();
		const AActor* BotActor = BotController->GetPawn();
		
		if (TargetActor && BotController && BotActor)
		{
			const float Distance = FVector::Distance(TargetActor->GetActorLocation(), BotActor->GetActorLocation());
			const bool bWithinRange = Distance < MaxAttackRange;

			bool bHasLineOfSight = false;
			if (bWithinRange)
			{
				bHasLineOfSight = BotController->LineOfSightTo(TargetActor);
			}

			BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLineOfSight));
		}	
	}
}
