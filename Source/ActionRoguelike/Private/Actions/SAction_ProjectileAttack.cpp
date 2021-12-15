// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_ProjectileAttack.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "SBaseProjectile.h"

static TAutoConsoleVariable<bool> CVarDrawProjectileDebug(TEXT("su.DrawProjectileDebug"), false, TEXT("Drawing debug information about projectile attacks."), ECVF_Cheat);

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
    MuzzleSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    const auto Character = Cast<ACharacter>(InstigatorActor);
    if(!Character) return;
    
    if(GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_ProjectileAttack)) return;
    
    Character->PlayAnimMontage(ProjectileAttackMontage);

    FTimerDelegate ProjectileAttackDelegate;
    ProjectileAttackDelegate.BindUObject(this, &USAction_ProjectileAttack::ProjectileAttack_TimeElapsed, Character);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_ProjectileAttack, ProjectileAttackDelegate, 0.2f, false);
}

void USAction_ProjectileAttack::ProjectileAttack_TimeElapsed(ACharacter* InstigatorCharacter)
{
    if(!ensure(ProjectileClass)) return;

    /** LineTrace **/
    /** It helps to find the Rotation for Projectile **/

    FVector ViewPointLocation;
    FRotator ViewPointRotation;
    InstigatorCharacter->GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

    const auto StartTraceVector = ViewPointLocation;
    auto EndTraceVector = ViewPointLocation + (ViewPointRotation.Vector() * 2000.f);

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);
    
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByObjectType(HitResult, StartTraceVector, EndTraceVector, {}, CollisionQueryParams);

    if(HitResult.bBlockingHit)
    {
        EndTraceVector = HitResult.ImpactPoint;
    }
    
    if(CVarDrawProjectileDebug.GetValueOnGameThread())
    {
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 32, FColor::Green, false, 5.f, 0.f, 1.f);
        DrawDebugLine(GetWorld(), StartTraceVector, EndTraceVector, FColor::Green, false, 5.f,0.f,1.f);
    }
    
    /** End LineTrace **/
    
    const auto SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);
    const auto SpawnRotation = FRotationMatrix::MakeFromX(EndTraceVector-SpawnLocation).Rotator();

    const auto SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
    
    auto Projectile = GetWorld()->SpawnActorDeferred<ASBaseProjectile>(ProjectileClass, SpawnTransform, InstigatorCharacter, InstigatorCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    Projectile->MuzzleSocketName = MuzzleSocketName;

    Projectile->FinishSpawning(SpawnTransform);

    // Fixing the bug when The Projectile hit The Owner
    InstigatorCharacter->MoveIgnoreActorAdd(Projectile);

    StopAction(InstigatorCharacter);
}
