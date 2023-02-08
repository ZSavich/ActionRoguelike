// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SAction_Sprint.h"

#include "GameFramework/CharacterMovementComponent.h"

USAction_Sprint::USAction_Sprint()
{
	ActionName = "Sprint";
	SprintMultiply = 1.f;
}

bool USAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	if (Super::StartAction_Implementation(Instigator))
	{
		MovementComponent = MovementComponent ? MovementComponent : Instigator->FindComponentByClass<UCharacterMovementComponent>();
		if (MovementComponent)
		{
			MovementComponent->MaxWalkSpeed *= SprintMultiply;
			return true;
		}
	}
	return false;
}

bool USAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	if (Super::StopAction_Implementation(Instigator))
	{
		if (MovementComponent)
		{
			MovementComponent->MaxWalkSpeed /= SprintMultiply;
			return true;
		}
	}
	return false;
}


