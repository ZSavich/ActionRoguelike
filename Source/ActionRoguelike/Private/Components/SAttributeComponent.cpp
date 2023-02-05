// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SAttributeComponent.h"

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (ensure(FromActor))
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (ensure(Actor))
	{
		if (const USAttributeComponent* AttributeComponent = GetAttributes(Actor))
		{
			return AttributeComponent->IsAlive();
		}
	}
	return false;
}

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
	if (!GetOwner()->CanBeDamaged() || !IsAlive() || IsFullHealth() && Delta > 0.f)
	{
		return false;
	}

	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	const float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(Instigator, this, Health, ActualDelta);
	return ActualDelta != 0;
}

bool USAttributeComponent::KillSelf()
{
	if (IsAlive())
	{
		ApplyHealthChange(nullptr ,-MaxHealth);
		return true;
	}
	return false;
}
