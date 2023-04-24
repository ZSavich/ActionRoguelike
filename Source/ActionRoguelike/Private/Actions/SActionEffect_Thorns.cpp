// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SActionEffect_Thorns.h"

#include "SGameplayFunctionLibrary.h"
#include "Components/SAttributeComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogActionEffect, All, All);

USActionEffect_Thorns::USActionEffect_Thorns()
{
	bAutoStart = true;
}

bool USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	if (!USAction::StartAction_Implementation(Instigator)) return false;

	if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Instigator))
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::ExecuteThornEffect);
		return true;
	}
	return false;
}

bool USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	if (USAction::StopAction_Implementation(Instigator)) return false;

	if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Instigator))
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::ExecuteThornEffect);
		return true;
	}
	return false;
}

void USActionEffect_Thorns::ExecuteThornEffect(AActor* Instigator, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (!Instigator || Delta >= 0.f || NewHealth == 0.f) return;
	
	if (Instigator == OwningComp->GetOwner())
	{
		UE_LOG(LogActionEffect, Display, TEXT("Thorn Action Effect can't reflect damage caused to yourself."))
		return;
	}

	// Heal the owner of the thorns
	OwningComp->ApplyHealthChange(nullptr, -Delta);

	// Apply Damage to the Instigator Enemy
	if (USAttributeComponent* EnemyAttributes = USAttributeComponent::GetAttributes(Instigator))
	{
		if (EnemyAttributes->GetHealth() + Delta > 0.f)
		{
			EnemyAttributes->ApplyHealthChange(OwningComp->GetOwner(), Delta);
		}
	}
}
