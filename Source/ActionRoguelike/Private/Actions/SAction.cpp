// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SAction.h"

USAction::USAction()
{
	bIsActive = false;
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
	if (!bIsActive)
	{
		if (const USActionComponent* ActionComponent = GetOwningComponent())
		{
			const bool bHasBlockedTags = ActionComponent->ActiveGameplayTags.HasAny(BlockedTags);
			return !bHasBlockedTags;
		}
	}
	return false;
}

bool USAction::StartAction_Implementation(AActor* Instigator)
{
	if (!bIsActive && Instigator)
	{
		if (USActionComponent* ActionComponent = GetOwningComponent())
		{
			ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
			bIsActive = true;
			
			UE_LOG(LogTemp, Log, TEXT("EUD::Started action %s"), *GetNameSafe(this));
			return true;
		}
	}
	return false;
}

bool USAction::StopAction_Implementation(AActor* Instigator)
{
	if (bIsActive && Instigator)
	{
		if (USActionComponent* ActionComponent = GetOwningComponent())
		{
			ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
			bIsActive = false;
			
			UE_LOG(LogTemp, Log, TEXT("EUD::Stopped action %s"), *GetNameSafe(this));
			return true;
		}
	}
	return false;
}