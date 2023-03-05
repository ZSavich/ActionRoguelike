// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SHealthPotion.h"
#include "Components/SAttributeComponent.h"

#define LOCTEXT_NAMESPACE "SInteractableActors"

ASHealthPotion::ASHealthPotion()
{
	bCanRespawn = true;
	RespawnTime = 10.f;
	HealValue = 100.f;
}

bool ASHealthPotion::ActivatePickup(APawn* InstigatorPawn)
{
	if (!InstigatorPawn || HealValue <= 0.f) return false;
	
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		if (AttributeComponent->IsFullHealth()) return false;

		if (Super::ActivatePickup(InstigatorPawn))
		{
			AttributeComponent->ApplyHealthChange(InstigatorPawn, HealValue);
			return true;
		}
	}
	return false;
}

FText ASHealthPotion::GetInteractText(APawn* InstigatorPawn)
{
	if (const USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn))
	{
		if (AttributeComponent->IsFullHealth())
		{
			// NSLOCTEXT("SInteractableActors", "HealthPotion_FullHealthWarning", "Already at full health.");
			return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
		}
		else
		{
			// FText::Format(NSLOCTEXT("SInteractableActors", "HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CostCredits);
			return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CostCredits);
		}
	}
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE