// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!ensure(ProjectileClass)) return EBTNodeResult::Failed;
    
    const auto TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    if(!TargetActor) return EBTNodeResult::Failed;

    const auto AIController = OwnerComp.GetAIOwner();
    if(!ensure(AIController)) return EBTNodeResult::Failed;

    const auto AICharacter = AIController->GetCharacter();
    if(!ensure(AICharacter)) return EBTNodeResult::Failed;

    const auto MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);
    
    const auto Direction = TargetActor->GetActorLocation() - MuzzleLocation;
    const auto MuzzleRotation = Direction.Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    const auto ProjectileActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

    return ProjectileActor ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
