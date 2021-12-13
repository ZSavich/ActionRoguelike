// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pickups/SHealthPotion.h"

#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
    HealCount = 25.f;
}

bool ASHealthPotion::Effect(AActor* InstigatorActor)
{
    const auto AttributeComponent = USAttributeComponent::GetAttributes(InstigatorActor);
    if(!AttributeComponent) return false;

    const auto HealResult = AttributeComponent->ApplyHealthChange(this, HealCount);
    return HealResult;
}