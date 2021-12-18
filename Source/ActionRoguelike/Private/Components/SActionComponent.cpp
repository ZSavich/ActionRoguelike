// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SActionComponent.h"

#include "SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

    for(const auto ActionClass : ActionClasses)
    {
        AddAction(GetOwner(), ActionClass);
    }
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple(); 
    GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, DebugMsg);
}

void USActionComponent::AddAction(AActor* InstigatorActor , TSubclassOf<USAction> ActionClass)
{
    if(!ensure(ActionClass)) return;

    const auto ActionInstance = NewObject<USAction>(this, ActionClass);
    if(!ensure(ActionInstance)) return;

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
            Action->StopAction(InstigatorActor);
            return true;
        }
    }
    return false;
}

