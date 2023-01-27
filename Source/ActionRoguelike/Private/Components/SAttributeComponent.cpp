// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	Health = 100.f;
	MaxHealth = 100.f;
}

bool USAttributeComponent::ApplyHealthChange(AActor* Instigator, const float Delta)
{
	if (Health <= 0)
	{
		return false;
	}
	
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Instigator, this, Health, Delta);
	return true;
}
