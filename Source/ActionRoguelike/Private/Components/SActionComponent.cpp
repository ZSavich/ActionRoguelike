// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/SAction.h"

// Static Functions - Start
USActionComponent* USActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USActionComponent>();
	}
	return nullptr;
}
// Static Functions - End

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Debug active gameplay tage
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("GameplayTags: %s"), *ActiveGameplayTags.ToStringSimple()));
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

USAction* USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (ActionClass)
	{
		if (USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass))
		{
			ActiveActions.AddUnique(NewAction);

			if (NewAction->ShouldAutoStart())
			{
				NewAction->StartAction(GetOwner());
			}
			return NewAction;
		}
	}
	return nullptr;
}

void USActionComponent::RemoveAction(USAction* Action)
{
	if (Action && !Action->IsRunning())
	{
		ActiveActions.Remove(Action);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, const FName& ActionName)
{
	if (Instigator == nullptr || ActionName.IsNone()) return false;
	
	for (USAction*& Action : ActiveActions)
	{
		if (Action->GetActionName() == ActionName && Action->CanStart())
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
		if (Action->GetActionName() == ActionName && Action->IsRunning())
		{
			return Action->StopAction(Instigator);
		}
	}
	return false;
}
