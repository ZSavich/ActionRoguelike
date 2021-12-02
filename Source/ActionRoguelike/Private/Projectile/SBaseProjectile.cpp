// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SBaseProjectile.h"

#include "SCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASBaseProjectile::ASBaseProjectile()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBaseProjectile::OnActorOverlap);
    SphereComp->SetCollisionProfileName("Projectile");
    SetRootComponent(SphereComp);
    
    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
    EffectComp->SetupAttachment(GetRootComponent());
    
    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    MovementComp->InitialSpeed = 4000.f;
    MovementComp->ProjectileGravityScale = 0.2;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;

    FlightSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FlightAudio"));
    FlightSound->SetupAttachment(GetRootComponent());

    LifeSpan = 10.f;
}

void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

    if(!ensure(FlightSound) || !ensure(EffectComp)) return;
    
    SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
    FlightSound->Activate();
    
    if(ensure(MuzzleFlashEffect))
    {
        const auto SCharacter = GetInstigator<ASCharacter>();
        if(SCharacter)
        {
            UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, SCharacter->GetMesh(), SCharacter->MuzzleSocketName);
        }
    }
    
    SetLifeSpan(LifeSpan);
}

void ASBaseProjectile::SpawnImpactEffects()
{
    if(ensure(ExplosionEffect))
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorTransform());
    }
    
    if(ensure(CameraShake))
    {
        UGameplayStatics::PlayWorldCameraShake(GetWorld(),CameraShake,GetActorLocation(),150.f, 300.f);
    }

    if(ensure(ImpactSound))
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
    }
}

void ASBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    SpawnImpactEffects();
    Destroy();
}
