// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SMagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Create a sphere component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);
	
	// Create a particle system component
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(GetRootComponent());

	// Create a projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f; // The Projectile should move forward 
	ProjectileMovementComponent->InitialSpeed = 1000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // Match direction of the projectile's velocity
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
