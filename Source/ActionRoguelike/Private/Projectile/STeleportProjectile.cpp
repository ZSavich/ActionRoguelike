// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASTeleportProjectile::ASTeleportProjectile()
{
}

void ASTeleportProjectile::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_Explodes, this, &ASTeleportProjectile::Explodes, 0.2f);
}

void ASTeleportProjectile::Explodes()
{
    ensure(ExplosionEffect);
    
    if(GetWorldTimerManager().IsTimerActive(TimerHandle_Explodes))
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Explodes);
    }
    
    EffectComp->Deactivate();
    MovementComp->Deactivate();
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionEffect, GetActorLocation());

    GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::Teleport, 0.2f);
}

void ASTeleportProjectile::Teleport()
{
    GetInstigator()->SetActorLocation(GetActorLocation());
    Destroy();
}
