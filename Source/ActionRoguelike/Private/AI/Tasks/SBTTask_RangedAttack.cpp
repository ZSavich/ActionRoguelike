// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
    MuzzleSocketName = "Muzzle_01";
    MaxBulletSpread = 5.f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!ensure(ProjectileClass)) return EBTNodeResult::Failed;
    
    const auto TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    if(!TargetActor || !USAttributeComponent::IsActorAlive(TargetActor)) return EBTNodeResult::Failed;

    const auto AIController = OwnerComp.GetAIOwner();
    if(!ensure(AIController)) return EBTNodeResult::Failed;

    const auto AICharacter = AIController->GetCharacter();
    if(!ensure(AICharacter)) return EBTNodeResult::Failed;

    const auto MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);
    
    const auto Direction = TargetActor->GetActorLocation() - MuzzleLocation;
    auto MuzzleRotation = Direction.Rotation();

    MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
    MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = AICharacter;
    
    const auto ProjectileActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

    return ProjectileActor ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
