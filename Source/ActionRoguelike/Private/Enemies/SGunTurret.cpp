// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SGunTurret.h"

#include "EngineUtils.h"
#include "SCharacter.h"
#include "SBaseProjectile.h"

ASGunTurret::ASGunTurret()
{
    AmountOfAttacksInRow = 3;
    CountOfAttacks = 0;
}

void ASGunTurret::BeginPlay()
{
	Super::BeginPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_LocateTarget, this, &ASGunTurret::LocateTarget, 2.f, true, 2.f);    
}

void ASGunTurret::LocateTarget()
{
    if(!ensure(ProjectileClass)) return;
    if(!TargetPawn)
    {
        for(TActorIterator<ASCharacter> It(GetWorld()); It; ++It)
        {
            TargetPawn = *It;
            break;
        }
        if(!ensure(TargetPawn)) return;
    }

    GetWorldTimerManager().SetTimer(TimerHandle_AttackTarget, this, &ASGunTurret::FindAndAttackTarget, 0.25f, true, 0.f);
}

void ASGunTurret::FindAndAttackTarget()
{
    if(!ensure(TargetPawn)) return;

    if(++CountOfAttacks >= AmountOfAttacksInRow)
    {
        CountOfAttacks = 0;
        GetWorldTimerManager().ClearTimer(TimerHandle_AttackTarget);
    }
    
    const auto TargetLocation = TargetPawn->GetActorLocation();
    const auto TurretRotation = FRotationMatrix::MakeFromX(TargetLocation - GetActorLocation()).Rotator();
    SetActorRotation(TurretRotation);

    GetWorld()->SpawnActor<ASBaseProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
}