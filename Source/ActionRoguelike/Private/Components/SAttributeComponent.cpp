// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(const float Delta)
{    
    CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.f, CurrentHealth);
    OnHealthChanged.Broadcast(nullptr, this, CurrentHealth,Delta);
    
    return true;
}
