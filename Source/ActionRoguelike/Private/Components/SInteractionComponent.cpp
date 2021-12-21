// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(TEXT("su.DrawDebugInteraction"), false, TEXT("Draw lines and spheres to debug the interaction component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    TraceRadius = 30.f;
    TraceDistance = 500.f;
    CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(GetOwner<APawn>()->IsLocallyControlled())
    {
        FindBestInteractable();
    }
}

void USInteractionComponent::FindBestInteractable()
{
    APawn* Owner = GetOwner<APawn>();
    if(!Owner) return;

    const auto Controller = Owner->Controller;
    if(!Controller) return;
    
    FVector EyeLocation;
    FRotator EyeRotation;
    
    Controller->GetPlayerViewPoint(EyeLocation, EyeRotation);
    
    const FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

    /*
     *  Way 1. A bad way for consoles where you need to be less accurate (LineTrace has 1px thickness)
     * 
    FHitResult HitResult;
    const auto bIsBlocking = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLocation, End, ObjectQueryParams);
    *
    */

    /** Way 2. Best way because it has interaction radius **/
    
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(TraceRadius);

    FocusedActor = nullptr;
    
    TArray<FHitResult> HitResults;
    const auto bIsBlocking = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
    
    const FColor LineColor = bIsBlocking ? FColor::Green : FColor::Red;

    for(const auto HitResult : HitResults)
    {
        const auto HitActor = HitResult.GetActor();
        if(HitActor && HitActor->Implements<USGameplayInterface>())
        {
            FocusedActor = HitActor;
            break;
        }
    }

    if(FocusedActor)
    {
        if(!DefaultWidgetInstance && ensure(DefaultWidgetClass))
        {
            DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
        }
        if(DefaultWidgetInstance)
        {
            DefaultWidgetInstance->AttachedActor = FocusedActor;
            if(!DefaultWidgetInstance->IsInViewport())
            {
                DefaultWidgetInstance->AddToViewport();
            }
        }
    }
    else
    {
        if(DefaultWidgetInstance)
        {
            DefaultWidgetInstance->RemoveFromParent();
        }
    }
    
    if(CVarDrawDebugInteraction.GetValueOnGameThread())
        DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 5.f, 0.f, 2.f);
}


void USInteractionComponent::PrimaryInteract()
{
    ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* ActorInteract)
{
    if(!ActorInteract)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No Focus Actor to interact.");
        return;
    }
    
    ISGameplayInterface::Execute_Interact(ActorInteract, GetOwner<APawn>());
}
