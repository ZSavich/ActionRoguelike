// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	AutoExplodeTime = 0.2f;

	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_AutoExplode, this, &ASDashProjectile::Explode, AutoExplodeTime, false);
}

void ASDashProjectile::Teleport()
{
	if (TimerHandle_Teleport.IsValid())
	{
		TimerHandle_Teleport.Invalidate();
	}

	// Teleport the instigator in appropriate location if it is possible
	if (GetInstigator())
	{
		GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	
	Destroy();
}

void ASDashProjectile::Explode()
{
	if (TimerHandle_AutoExplode.IsValid())
	{
		TimerHandle_AutoExplode.Invalidate();
	}

	// Make sure we 'stop' the projectile movement while we wait
	ProjectileMovementComponent->StopMovementImmediately();

	// Spawn portal effect where the character will appear 
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation());
	}

	// Hide a projectile effect while we wait for teleportation
	if (EffectComponent)
	{
		EffectComponent->Deactivate();
	}

	// Teleport the instigator in 0.2 seconds
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::Teleport, AutoExplodeTime, false);
}
