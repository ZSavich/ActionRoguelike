// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SActionComponent.h"

#include "SAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

    if(GetOwner()->GetLocalRole() == ROLE_Authority)
    {
        for(const auto ActionClass : ActionClasses)
        {
            AddAction(GetOwner(), ActionClass);
        }
    }
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
    for(auto Action: Actions)
    {
        FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
        FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),
            *GetNameSafe(GetOwner()),
            *GetNameSafe(Action));
        LogOnScreen(this, ActionMsg, TextColor, 0.f);
    }
}

void USActionComponent::AddAction(AActor* InstigatorActor , TSubclassOf<USAction> ActionClass)
{
    if(!ensure(ActionClass)) return;

    if(!GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("Warning! Action %s started on the client"), *GetNameSafe(ActionClass));
        return;
    }
    
    const auto ActionInstance = NewObject<USAction>(GetOwner(), ActionClass);
    
    if(!ensure(ActionInstance)) return;
    
    ActionInstance->Initialize(this);
    Actions.Add(ActionInstance);

    if(ActionInstance->bAutoStart)
        ActionInstance->StartAction(InstigatorActor);
}

void USActionComponent::RemoveAction(USAction* Action)
{
    if(!ensure(Action) || !Actions.Find(Action)) return;
    Actions.Remove(Action);
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, const FName& ActionName)
{
    if(!InstigatorActor) return false;
    
    for(const auto Action : Actions)
    {
        if(Action && Action->ActionName == ActionName)
        {
            if(Action->CanStart())
            {
                if(GetOwner()->GetLocalRole() != ROLE_Authority)
                {
                    ServerStartAction(InstigatorActor, ActionName);
                }
                Action->StartAction(InstigatorActor);
                return true;
            }
        }
    }
    return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, const FName& ActionName)
{
    for(const auto Action : Actions)
    {
        if(Action && Action->ActionName == ActionName && Action->IsRunning())
        {
            if(GetOwner()->GetLocalRole() != ROLE_Authority)
            {
                ServerStopAction(InstigatorActor, ActionName);
            }
            Action->StopAction(InstigatorActor);
            return true;
        }
    }
    return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName Action)
{
    StartActionByName(InstigatorActor, Action);
}

void USActionComponent::ServerStopAction_Implementation(AActor* InstigatorActor, FName Action)
{
    StopActionByName(InstigatorActor, Action);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
    bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    for(const auto Action: Actions)
        if(Action)
            Result |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
        
    return Result;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USActionComponent, Actions);
}