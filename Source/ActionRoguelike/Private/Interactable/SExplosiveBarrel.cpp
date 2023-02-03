// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/SExplosiveBarrel.h"

#include "Components/SAttributeComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Create a static mesh for the barrel actor
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetCollisionObjectType(ECC_PhysicsBody);
	SetRootComponent(BarrelMesh);

	// Create a radial force component to setup explosive strength
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->Radius = 750.f;
	RadialForceComponent->ImpulseStrength = 2500.f;
	RadialForceComponent->ForceStrength = 1000.f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComponent->SetupAttachment(GetRootComponent());

	// Default properties
	Damage = 50.f;
	DamageRadius = 250.f;
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	if (BarrelMesh)
	{
		BarrelMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnBarrelHit);
	}
}

void ASExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (RadialForceComponent)
	{
		RadialForceComponent->FireImpulse();
	}

	// Check if explosive overlap any actors with attribute component
	if (Damage > 0)
	{
		TArray<FOverlapResult> OverlapResults;
		
		FCollisionShape Shape;
		Shape.SetSphere(DamageRadius);
		
		GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape);
		for (const FOverlapResult& Result : OverlapResults)
		{
			if (AActor* Target = Result.GetActor())
			{
				if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Target))
				{
					AttributeComponent->ApplyHealthChange(this, -Damage);
				}
			}
		}
	}
}

