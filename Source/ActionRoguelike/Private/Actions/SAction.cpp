// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"
#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"


USAction::USAction()
{
    bAutoStart = false;
    bIsRunning = false;
}

void USAction::Initialize(USActionComponent* NewActionComp)
{
    ActionComp = NewActionComp;
}

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
    //UE_LOG(LogTemp, Display, TEXT("Startup Action: %s"), *GetNameSafe(this));
    LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

    if(GrandsTags.IsValid())
    {
        const auto OwnerComp = GetOwningComponent();
        if(OwnerComp)
            OwnerComp->ActiveGameplayTags.AppendTags(GrandsTags);
    }
    
    bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
    //UE_LOG(LogTemp, Display, TEXT("Endup Action: %s"), *GetNameSafe(this));
    LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
    
    const auto OwnerComp = GetOwningComponent();
    if(OwnerComp)
        OwnerComp->ActiveGameplayTags.RemoveTags(GrandsTags);
    
    bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
    // Outer is sets when creating action via NewObject<T>
    /*const auto Comp = GetOwningComponent();
    if(!Comp) return nullptr;*/
    
    AActor* Actor = Cast<AActor>(GetOuter());
    return Actor ? Actor->GetWorld() : nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
    return ActionComp;
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

void USAction::OnRep_IsRunning()
{
    if(bIsRunning)
        StartAction(nullptr);
    else
        StopAction(nullptr);
}

void USAction::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USAction, bIsRunning);
    DOREPLIFETIME(USAction, ActionComp);
}

