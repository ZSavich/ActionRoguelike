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
	// Make sure the owner is alive and can be cured
	if (!IsAlive() || IsFullHealth() && Delta > 0.f)
	{
		return false;
	}

	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	const float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(Instigator, this, Health, ActualDelta);
	return true;
}
