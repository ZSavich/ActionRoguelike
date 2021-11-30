// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SBlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    SphereComp->SetSphereRadius(75.f);
    SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
    
    MovementComp->ProjectileGravityScale = 0.f;
    MovementComp->InitialSpeed = 500.f;

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
    RadialForceComp->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    RadialForceComp->Falloff = ERadialImpulseFalloff::RIF_Linear;
    RadialForceComp->ImpulseStrength = 0.f;
    RadialForceComp->Radius = 750.f;
    RadialForceComp->ForceStrength = -1000000.f;
    RadialForceComp->SetupAttachment(SphereComp);

    LifeTime = 5.f;
}

void ASBlackholeProjectile::BeginPlay()
{
    Super::BeginPlay();

    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackholeProjectile::OnProjectileBeginOverlap);
    SetLifeSpan(5.f);
}

void ASBlackholeProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    OtherActor->Destroy();
}
