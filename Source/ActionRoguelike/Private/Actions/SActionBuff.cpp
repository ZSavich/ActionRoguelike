// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionBuff.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionBuff::USActionBuff()
{
    Period = 0.f;
}


void USActionBuff::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    const auto Owner = GetOwningComponent()->GetOwner();
    if(!Owner) return;

    const auto Attributes = USAttributeComponent::GetAttributes(Owner);
    if(!Attributes) return;

    Attributes->OnHealthChanged.AddDynamic(this, &USActionBuff::OnHealthChangedHandle);
}

void USActionBuff::StopAction_Implementation(AActor* InstigatorActor)
{
    const auto Owner = GetOwningComponent()->GetOwner();
    if(!Owner) return;

    const auto Attributes = USAttributeComponent::GetAttributes(Owner);
    if(!Attributes) return;

    Attributes->OnHealthChanged.RemoveDynamic(this, &USActionBuff::OnHealthChangedHandle);
    
    Super::StopAction_Implementation(InstigatorActor);
}

void USActionBuff::OnHealthChangedHandle(AActor* InstigatorActor, USAttributeComponent* AttributeComp, float CurrentHealth, float Delta)
{
    if(!InstigatorActor) return;
    
    USGameplayFunctionLibrary::ApplyDamage(InstigatorActor, Delta * -ReflectedDamage, GetOwningComponent()->GetOwner());
}