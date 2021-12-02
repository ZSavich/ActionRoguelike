// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

USInteractionComponent::USInteractionComponent()
{
    InteractionRadius = 15.f;
    bDrawDebugInformation = false;
}


void USInteractionComponent::PrimaryInteract() const
{
    APawn* Owner = GetOwner<APawn>();
    
    FVector EyeLocation;
    FRotator EyeRotation;
    Owner->Controller->GetPlayerViewPoint(EyeLocation, EyeRotation);
    
    const FVector End = EyeLocation + (EyeRotation.Vector() * 500.f);

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    /*
     *  Way 1. A bad way for consoles where you need to be less accurate (LineTrace has 1px thickness)
     * 
    FHitResult HitResult;
    const auto bIsBlocking = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLocation, End, ObjectQueryParams);
    *
    */

    /** Way 2. Best way because it has interaction radius **/
    
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(InteractionRadius);
    
    TArray<FHitResult> HitResults;
    const auto bIsBlocking = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
    
    const FColor LineColor = bIsBlocking ? FColor::Green : FColor::Red;

    for(const auto HitResult : HitResults)
    {
        const auto HitActor = HitResult.GetActor();
        if(HitActor && HitActor->Implements<USGameplayInterface>())
        {
            ISGameplayInterface::Execute_Interact(HitActor, Owner);

            if(bDrawDebugInformation)
                DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, InteractionRadius,32, LineColor, false, 5.f, 0.f, 2.f);
            
            break;
        }
    }
    if(bDrawDebugInformation)
        DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 5.f, 0.f, 2.f);
}

