// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SAttributeComponent.h"


USAttributeComponent::USAttributeComponent()
{
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
    return CurrentHealth > 0.f; 
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
    if(!IsAlive()) return false;
    
    if(CurrentHealth == MaxHealth && Delta >= 0.f)
    {
        OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth,Delta);
        return false;
    }

    const auto OldHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.f, MaxHealth);
    const auto ActualDelta = CurrentHealth - OldHealth;
    
    OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, ActualDelta);

    if(!IsAlive())
    {
        OnDead.Broadcast(InstigatorActor, GetOwner());
    }
    
    return true;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* Actor)
{
    if(!Actor) return nullptr;

    const auto AttributeComp = Actor->FindComponentByClass<USAttributeComponent>();
    if(!AttributeComp) return nullptr;

    return AttributeComp;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
    if(!Actor) return false;

    const auto AttributeComp = GetAttributes(Actor);
    if(!AttributeComp || !AttributeComp->IsAlive()) return false;

    return true;
}