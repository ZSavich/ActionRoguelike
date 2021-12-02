// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASTeleportProjectile::ASTeleportProjectile()
{
    ExplosiveDelay = 0.2f;
    TeleportDelay = 0.2f;
}

void ASTeleportProjectile::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_Explodes, this, &ASTeleportProjectile::Explodes, ExplosiveDelay);
}

void ASTeleportProjectile::Explodes()
{
    if(!ensure(ExplosionEffect)) return;
    
    GetWorldTimerManager().ClearTimer(TimerHandle_Explodes);
    
    EffectComp->Deactivate();
    MovementComp->StopMovementImmediately();
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

    GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::Teleport, TeleportDelay);
}

void ASTeleportProjectile::Teleport()
{
    const auto InstigatorPawn = GetInstigator(); 
    if(!InstigatorPawn) return;

    InstigatorPawn->TeleportTo(GetActorLocation(), InstigatorPawn->GetActorRotation());
    Destroy();
}

void ASTeleportProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Explodes();
}
