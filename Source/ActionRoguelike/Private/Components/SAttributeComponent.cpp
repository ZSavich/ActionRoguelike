// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Static Functions - Start
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
// Static Functions - End

USAttributeComponent::USAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	Health = 100.f;
	MaxHealth = 100.f;
	Rage = 10.f;
	MaxRage = 100.f;

	SetIsReplicatedByDefault(true);
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
	const float NewHealth = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	const float ActualDelta = NewHealth - OldHealth;

	// Only on the Server
	if (GetOwnerRole() == ROLE_Authority)
	{
		Health = NewHealth;

		if (ActualDelta < 0.f)
		{
			if (FMath::IsNearlyZero(Health))
			{
				if (ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>())
				{
					GameMode->OnActorKilled(GetOwner(), Instigator);
				}
			}
			else
			{
				ApplyRageChange(-ActualDelta);
			}
		}
		
		MulticastOnHealthChanged(Instigator, Health, ActualDelta);
	}
	
	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(float Delta)
{
	// Make sure the owner is alive
	if (!IsAlive())
	{
		return false;
	}

	const float OldRage = Rage;
	const float NewRage = FMath::Clamp(Rage + Delta, 0.f, MaxRage);
	const float ActualDelta = NewRage - OldRage;

	if (GetOwnerRole() == ROLE_Authority)
	{
		Rage = NewRage;
		MulticastOnRageChanged(Rage, ActualDelta);
	}

	return ActualDelta != 0.f;
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

// Multiplayer Functions - START
void USAttributeComponent::MulticastOnHealthChanged_Implementation(AActor* Instigator, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastOnRageChanged_Implementation(float NewRage, float Delta)
{
	OnRageChanged.Broadcast(this, NewRage, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate Health Attribute
	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	// Replicate Rage Attribute
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, MaxRage);
}
// Multiplayer Functions - END