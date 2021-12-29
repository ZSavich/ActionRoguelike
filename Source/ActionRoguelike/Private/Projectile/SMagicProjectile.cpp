// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SMagicProjectile.h"

#include "SActionComponent.h"
#include "SActionEffect.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
    DamageAmount = 20.f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    const auto ActionComp = OtherActor->FindComponentByClass<USActionComponent>();
    if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTagName))
    {
        MovementComp->Velocity = -MovementComp->Velocity;
        SetInstigator(Cast<APawn>(OtherActor));
        return;
    }
    
    if(USGameplayFunctionLibrary::ApplyDirectionalDamage(OtherComp, OtherActor, DamageAmount, GetInstigator(), SweepResult))
    {
        SpawnImpactEffects();
        
        if(ensure(BurningActionClass) && GetOwner()->HasAuthority())
        {
            const auto TargetActionComp = OtherActor->FindComponentByClass<USActionComponent>();
            if(TargetActionComp)
            {
                TargetActionComp->AddAction(GetInstigator(),BurningActionClass);
            }
        }
    }
        
    Super::OnActorOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASMagicProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
    FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    SpawnImpactEffects();
    Destroy();
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}
