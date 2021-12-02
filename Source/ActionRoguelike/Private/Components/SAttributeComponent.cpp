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

bool USAttributeComponent::ApplyHealthChange(const float Delta)
{
    if(CurrentHealth == MaxHealth && Delta >= 0.f)
    {
        OnHealthChanged.Broadcast(nullptr, this, CurrentHealth,Delta);
        return false;
    }
    
    CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(nullptr, this, CurrentHealth, Delta);
    return true;
}
