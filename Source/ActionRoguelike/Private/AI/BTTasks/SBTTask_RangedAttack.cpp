// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTasks/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Projectiles/SProjectileBase.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MuzzleSocketName = "Muzzle_Front";
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ensureMsgf(ProjectileClass, TEXT("EUD::ProjectileClass is empty! Please set the ProjectileClass for %s"), *GetNameSafe(this)))
	{
		const AAIController* OwnerController = OwnerComp.GetAIOwner();
		if (ensure(OwnerController))
		{
			if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerController->GetPawn()))
			{
				const FVector MuzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);

				if (const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
				{
					if (const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor")))
					{
						// Find direction from Bot's muzzle to the Target Actor's location
						const FVector MuzzleDirection = TargetActor->GetActorLocation() - MuzzleLocation;
						const FRotator MuzzleRotation = MuzzleDirection.Rotation();
						
						FActorSpawnParameters SpawnParameters;
						SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						SpawnParameters.Instigator = OwnerCharacter;
						
						if (GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters))
						{
							return Super::ExecuteTask(OwnerComp, NodeMemory);
						}
					}
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
