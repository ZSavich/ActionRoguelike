// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser)
{
    const auto AttributeComp = USAttributeComponent::GetAttributes(DamagedActor);
    if(!AttributeComp) return false;

    return AttributeComp->ApplyHealthChange(DamageCauser, -BaseDamage);
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(UPrimitiveComponent* Component, AActor* DamagedActor, float BaseDamage, AActor* DamageCauser,
    const FHitResult& HitResult)
{
    if(!ApplyDamage(DamagedActor, BaseDamage, DamageCauser)) return false;
    
    if(HitResult.BoneName.IsValid() && Component && Component->IsSimulatingPhysics(HitResult.BoneName))
    {
        constexpr auto Magnitude = 300000.f;
        const auto Direction = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
        Component->AddImpulse(Direction * Magnitude, HitResult.BoneName);
    }
    return true;
}


