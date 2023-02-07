// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

bool USAttributeComponent::ApplyHealthChange(AActor* Instigator, float Delta)
{
	// Make sure the owner is alive and can be cured
	if (!GetOwner()->CanBeDamaged() || !IsAlive() || (IsFullHealth() && Delta > 0.f))
	{
		return false;
	}

	// Multiply Damage via CVar "su.DamageMultiplier" (Default = 1.f)
	const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
	Delta *= DamageMultiplier;
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	const float ActualDelta = Health - OldHealth;

	if (ActualDelta < 0.f && FMath::IsNearlyZero(Health))
	{
		if (ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>())
		{
			GameMode->OnActorKilled(GetOwner(), Instigator);
		}
	}
	
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
