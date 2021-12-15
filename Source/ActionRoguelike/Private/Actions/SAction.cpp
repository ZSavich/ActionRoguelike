// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Display, TEXT("Startup Action: %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Display, TEXT("Endup Action: %s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const
{
    // Outer is sets when creating action via NewObject<T>
    const auto Comp = Cast<UActorComponent>(GetOuter());
    if(!Comp) return nullptr;

    return Comp->GetWorld();
}
