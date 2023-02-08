// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SActionComponent.h"
#include "Actions/SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Setup default actions to owner
	for (const TSubclassOf<USAction>& Action : DefaultActions)
	{
		AddAction(Action);
	}
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (ActionClass)
	{
		
		if (USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass))
		{
			ActiveActions.AddUnique(NewAction);
		}
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, const FName& ActionName)
{
	if (Instigator == nullptr || ActionName.IsNone()) return false;
	
	for (USAction*& Action : ActiveActions)
	{
		if (Action->GetActionName() == ActionName)
		{
			return Action->StartAction(Instigator);
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, const FName& ActionName)
{
	if (Instigator == nullptr || ActionName.IsNone()) return false;

	for (USAction*& Action : ActiveActions)
	{
		if (Action->GetActionName() == ActionName)
		{
			return Action->StopAction(Instigator);
		}
	}
	return false;
}
