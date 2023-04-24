// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SAction.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogAction, All, All);

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
	if (ActionRepData.bIsRunning) return false;
	
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

		UE_LOG(LogAction, Display, TEXT("Activating - %s :: Has Block Tags - %s :: Can Affors - %s"), *GetNameSafe(this), bHasBlockedTags ? TEXT("Yes") : TEXT("No"), bCanAfford ? TEXT("Yes") : TEXT("No"));
		return !bHasBlockedTags && bCanAfford;
	}
	
	return false;
}

bool USAction::StartAction_Implementation(AActor* Instigator)
{
	if (!Instigator) return false;
	
	USActionComponent* ActionComponent = GetOwningComponent();
	check(ActionComponent);
		
	ActionRepData.bIsRunning = true;
	ActionRepData.Instigator = Instigator;

	ActionComponent->HandleActionStart(GrantsTags, this);
	UE_LOG(LogAction, Display, TEXT("Started action %s"), *GetNameSafe(this));
	
	return true;
}

bool USAction::StopAction_Implementation(AActor* Instigator)
{
	if (!Instigator) return false;

	USActionComponent* ActionComponent = GetOwningComponent();
	check(ActionComponent);
	
	ActionRepData.bIsRunning = false;
	ActionRepData.Instigator = Instigator;

	ActionComponent->HandleActionStop(GrantsTags, this);
	UE_LOG(LogAction, Display, TEXT("Stopped action %s"), *GetNameSafe(this));
	
	return true;
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