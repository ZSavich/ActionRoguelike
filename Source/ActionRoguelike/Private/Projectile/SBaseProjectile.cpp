// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASBaseProjectile::ASBaseProjectile()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereComp->SetCollisionProfileName("Projectile");
    SetRootComponent(SphereComp);
    
    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
    EffectComp->SetupAttachment(GetRootComponent());
    
    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    MovementComp->InitialSpeed = 4000.f;
    MovementComp->ProjectileGravityScale = 0.2;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;

    LifeSpan = 10.f;
}

void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
    
    SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
    SetLifeSpan(LifeSpan);
}
