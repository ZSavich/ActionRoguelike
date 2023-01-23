// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SInteractionComponent.h"

#include "Interfaces/SGameplayInterface.h"

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Set general properties
	TraceSphereRadius = 15.f;
	TraceLength = 1000.f;
	TraceObjectType = ECC_WorldDynamic;

	bDrawDebug = false;
}

void USInteractionComponent::PrimaryInteract()
{
	const UWorld* World = GetWorld();
	APawn* Owner = GetOwner<APawn>();
	
	if (World && Owner)
	{
		TArray<FHitResult> HitResults;
		
		FVector EyesLocation;
		FRotator EyesRotation;
		Owner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		const FVector EndHit = EyesLocation + (EyesRotation.Vector() * TraceLength);

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(TraceObjectType);

		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(TraceSphereRadius);
		
		const bool bIsHitActors = World->SweepMultiByObjectType(
			HitResults,
			EyesLocation,
			EndHit,
			FQuat::Identity,
			QueryParams,
			CollisionShape);

		if (bIsHitActors)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				AActor* HitActor = HitResult.GetActor();
				if (HitActor && HitActor->Implements<USGameplayInterface>())
				{
					ISGameplayInterface::Execute_Interact(HitActor, Owner);
					// Debug - Start
					if (bDrawDebug)
					{
						DrawDebugSphere(World, HitResult.ImpactPoint, TraceSphereRadius, 12, FColor::Green, false, 5.f, 0, 1.f);
					}
					// Debug - End
					break;
				}
			}
		}

		// Debug - Start
		if (bDrawDebug)
		{
			const FColor DebugLineColor = bIsHitActors ? FColor::Green : FColor::Red;
			DrawDebugLine(World, EyesLocation, EndHit, DebugLineColor, false, 5.f, 0, 1.f);
		}
		// Debug - End
	}
}
