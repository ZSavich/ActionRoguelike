// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SInteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "Interfaces/SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarInteractionDebugDraw(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// Set general properties
	TraceSphereRadius = 15.f;
	TraceLength = 5000.f;
	TraceObjectType = ECC_WorldDynamic;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerPawn && OwnerPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	const UWorld* World = GetWorld();
	APawn* Owner = GetOwner<APawn>();
	UCameraComponent* FollowCamera = Cast<UCameraComponent>(Owner->GetComponentByClass(UCameraComponent::StaticClass()));
	
	if (World && Owner)
	{
		const bool bDrawDebug = CVarInteractionDebugDraw.GetValueOnGameThread();
		
		TArray<FHitResult> HitResults;
		
		FVector EyesLocation;
		FRotator EyesRotation;
		Owner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		FVector EndHit;
		if (FollowCamera)
		{
			EyesLocation = FollowCamera->GetComponentLocation();
			EndHit = EyesLocation + (FollowCamera->GetComponentRotation().Vector() * TraceLength);
		}
		else
		{
			EndHit = EyesLocation + (EyesRotation.Vector() * TraceLength);
		}

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
		
		FocusedActor = nullptr;
		if (bIsHitActors)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				AActor* HitActor = HitResult.GetActor();
				if (HitActor && HitActor->Implements<USGameplayInterface>())
				{
					FocusedActor = HitActor;
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

		// Display the Interact Widget Popup when we are looking at the interactable actor
		if (FocusedActor)
		{
			if (!DefaultWidgetInstance && DefaultWidgetClass)
			{
				DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
			}
			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->SetAttachedActor(FocusedActor);
				if (!DefaultWidgetInstance->IsInViewport())
				{
					DefaultWidgetInstance->AddToViewport();
				}
			}
		}
		else
		{
			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->RemoveFromParent();
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

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

// RPC Functions
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocusActor)
{
	APawn* Owner = GetOwner<APawn>();
	if (Owner && InFocusActor)
	{
		ISGameplayInterface::Execute_Interact(InFocusActor, Owner);
	}
}
