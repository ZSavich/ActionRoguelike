// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"
#include "SActionComponent.h"


USAction::USAction()
{
    bAutoStart = false;
    bIsRunning = false;
}

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Display, TEXT("Startup Action: %s"), *GetNameSafe(this));

    const auto OwnerComp = GetOwningComponent();
    OwnerComp->ActiveGameplayTags.AppendTags(GrandsTags);

    bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Display, TEXT("Endup Action: %s"), *GetNameSafe(this));

    const auto OwnerComp = GetOwningComponent();
    OwnerComp->ActiveGameplayTags.RemoveTags(GrandsTags);

    bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
    // Outer is sets when creating action via NewObject<T>
    const auto Comp = Cast<UActorComponent>(GetOuter());
    if(!Comp) return nullptr;

    return Comp->GetWorld();
}

USActionComponent* USAction::GetOwningComponent() const
{
    return Cast<USActionComponent>(GetOuter());
}

bool USAction::CanStart_Implementation()
{
    if(IsRunning()) return false;

    const auto OwnerComp = GetOwningComponent();
    if (OwnerComp && OwnerComp->ActiveGameplayTags.HasAny(BlockedTags))
    {
        const FString DebugMsg = "Blocked Tag: " + ActionName.ToString();
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, DebugMsg);
        return false;
    }

    return true;
}
