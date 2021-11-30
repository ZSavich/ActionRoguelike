// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereComp->SetCollisionProfileName("Projectile");
    SetRootComponent(SphereComp);
    
    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
    EffectComp->SetupAttachment(GetRootComponent());
    
    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    MovementComp->InitialSpeed = 2000.f;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
    
    SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}