// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/SAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

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

	SetIsReplicatedByDefault(true);
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Debug active gameplay tage
	//GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("GameplayTags: %s"), *ActiveGameplayTags.ToStringSimple()));

	for (const USAction* Action : ActiveActions)
	{
		if (Action)
		{
			const FColor Color = Action->IsRunning() ? FColor::Blue : FColor::White;
			const FString Message = FString::Printf(TEXT("[%s] Action - %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
			LogOnScreen(this, Message, Color, 0.f);
		}
	}
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Setup default actions to owner
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (const TSubclassOf<USAction>& Action : DefaultActions)
		{
			AddAction(Action);
		}
	}
}

USAction* USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	// Call only on the Server
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Action %s trying activate on the client."), *GetNameSafe(this));
		return nullptr;
	}
	
	if (ActionClass)
	{
		if (USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass))
		{
			ActiveActions.AddUnique(NewAction);
			NewAction->Initialize(this);

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
			// Only on a Client
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
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
			// Only on a client
			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}
			
			return Action->StopAction(Instigator);
		}
	}
	return false;
}

void USActionComponent::HandleActionStart(const FGameplayTagContainer& GrantsTags, USAction* Action)
{
	ActiveGameplayTags.AppendTags(GrantsTags);
	OnActionStarted.Broadcast(this, Action);
}

void USActionComponent::HandleActionStop(const FGameplayTagContainer& GrantsTags, USAction* Action)
{
	ActiveGameplayTags.RemoveTags(GrantsTags);
	OnActionStopped.Broadcast(this, Action);
}

// Multiplayer Functions
bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bHaveReplicateChanges = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : ActiveActions)
	{
		if (Action)
		{
			bHaveReplicateChanges |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return bHaveReplicateChanges;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, const FName& ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, const FName& ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void USActionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, ActiveActions);
}