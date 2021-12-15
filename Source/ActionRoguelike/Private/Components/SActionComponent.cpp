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
        AddAction(ActionClass);
    }
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
    if(!ensure(ActionClass)) return;

    const auto ActionInstance = NewObject<USAction>(this, ActionClass);
    if(!ensure(ActionInstance)) return;

    Actions.Add(ActionInstance);
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, const FName& ActionName)
{
    if(!InstigatorActor) return false;
    
    for(const auto Action : Actions)
    {
        if(Action->ActionName == ActionName)
        {
            Action->StartAction(InstigatorActor);
            return true;
        }
    }
    return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, const FName& ActionName)
{
    for(const auto Action : Actions)
    {
        if(Action && Action->ActionName == ActionName)
        {
            Action->StopAction(InstigatorActor);
            return true;
        }
    }
    return false;
}

