// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SPickup_GrantAction.h"

#include "SCharacter.h"
#include "Actions/SAction.h"


void ASPickup_GrantAction::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (!ensure(GrantAction)) return;

	if (USActionComponent* ActionComponent = USActionComponent::GetActionComponent(InstigatorPawn))
	{
		if (const USAction* Action = ActionComponent->AddAction(GrantAction))
		{
			if (ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn))
			{
				Character->GrantActionName = Action->GetActionName();
			}
		}
	}
}
