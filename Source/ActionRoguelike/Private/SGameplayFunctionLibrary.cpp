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
    
    UE_LOG(LogTemp, Warning, TEXT("USGameplayFunctionLibrary  ApplyDirectionalDamage 1"));
    if(HitResult.BoneName.IsValid() && Component && Component->IsSimulatingPhysics(HitResult.BoneName))
    {
        UE_LOG(LogTemp, Warning, TEXT("USGameplayFunctionLibrary  ApplyDirectionalDamage 2"));
        const auto ImpulseMagnitude = 300000.f;
        const auto ImpulseVector = -HitResult.ImpactNormal * ImpulseMagnitude;
        Component->AddImpulse(ImpulseVector, HitResult.BoneName);
    }
    return true;
}


