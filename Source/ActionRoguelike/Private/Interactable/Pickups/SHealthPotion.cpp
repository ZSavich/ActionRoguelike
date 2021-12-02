// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pickups/SHealthPotion.h"

#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
    HealCount = 25.f;
}

bool ASHealthPotion::Effect(const APawn* InstigatorActor)
{
    const auto AttributeComponent = InstigatorActor->FindComponentByClass<USAttributeComponent>();
    if(!AttributeComponent) return false;

    const auto HealResult = AttributeComponent->ApplyHealthChange(HealCount);
    return HealResult;
}