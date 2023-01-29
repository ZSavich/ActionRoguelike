// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SBlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile()
{
	// Change speed properties
	ProjectileMovementComponent->InitialSpeed = 300.f;
	
	// Change collision properties for the Sphere Component
	SphereComponent->SetSphereRadius(12.f);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// Create a radial force component
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->Radius = 500.f;
	RadialForceComponent->ImpulseStrength = 0.f;
	RadialForceComponent->ForceStrength = -500000.f;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComponent->AddCollisionChannelToAffect(ECC_PhysicsBody);
	RadialForceComponent->SetupAttachment(GetRootComponent());
}

void ASBlackholeProjectile::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Destroy all 'simulating' actors on overlap
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}