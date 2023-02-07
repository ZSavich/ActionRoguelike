// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SHealthPotion.h"
#include "Components/SAttributeComponent.h"

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
