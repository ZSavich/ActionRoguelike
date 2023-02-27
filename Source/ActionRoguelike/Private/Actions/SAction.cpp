// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SAction.h"

#include "Components/SAttributeComponent.h"
#include "Net/UnrealNetwork.h"

USAction::USAction()
{
	ActionRepData.bIsRunning = false;
	bAutoStart = false;
	RageCost = 0.f;
}

void USAction::Initialize(USActionComponent* ActionComponent)
{
	OwnActionComponent = ActionComponent;
}

USActionComponent* USAction::GetOwningComponent() const
{
	if (AActor* OuterActor = Cast<AActor>(GetOuter()))
	{
		return Cast<USActionComponent>(OuterActor->GetComponentByClass(USActionComponent::StaticClass()));
	}
	return nullptr;
}

bool USAction::CanStart() const
{
	if (!ActionRepData.bIsRunning)
	{
		if (const USActionComponent* ActionComponent = GetOwningComponent())
		{
			const bool bHasBlockedTags = ActionComponent->ActiveGameplayTags.HasAny(BlockedTags);
			bool bCanAfford = true;
			
			if (RageCost > 0.f)
			{
				if (AActor* OuterActor = Cast<AActor>(GetOuter()))
				{
					if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(OuterActor))
					{
						bCanAfford = AttributeComponent->ApplyRageChange(-RageCost);
					}
				}
			}

			UE_LOG(LogTemp, Log, TEXT("EUD::Activating - %s :: Has Block Tags - %s :: Can Affors - %s"), *GetNameSafe(this), bHasBlockedTags ? TEXT("Yes") : TEXT("No"), bCanAfford ? TEXT("Yes") : TEXT("No"));
			return !bHasBlockedTags && bCanAfford;
		}
	}
	return false;
}

bool USAction::StartAction_Implementation(AActor* Instigator)
{
	if (Instigator)
	{
		if (USActionComponent* ActionComponent = GetOwningComponent())
		{
			ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
			ActionRepData.bIsRunning = true;
			ActionRepData.Instigator = Instigator;
			
			ActionComponent->OnActionStarted.Broadcast(ActionComponent, this);
			
			UE_LOG(LogTemp, Log, TEXT("EUD::Started action %s"), *GetNameSafe(this));
			return true;
		}
	}
	return false;
}

bool USAction::StopAction_Implementation(AActor* Instigator)
{
	if (Instigator)
	{
		if (USActionComponent* ActionComponent = GetOwningComponent())
		{
			ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
			ActionRepData.bIsRunning = false;
			ActionRepData.Instigator = Instigator;

			ActionComponent->OnActionStopped.Broadcast(ActionComponent, this);
			
			UE_LOG(LogTemp, Log, TEXT("EUD::Stopped action %s"), *GetNameSafe(this));
			return true;
		}
	}
	return false;
}

// Multiplayer Functions
void USAction::OnRep_ActionRepData()
{
	if (ActionRepData.bIsRunning)
	{
		StartAction(ActionRepData.Instigator);
	}
	else
	{
		StopAction(ActionRepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, ActionRepData);
	DOREPLIFETIME(USAction, OwnActionComponent)
}