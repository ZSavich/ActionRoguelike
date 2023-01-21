// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/SExplosiveBarrel.h"

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
	RadialForceComponent->Radius = 500.f;
	RadialForceComponent->ImpulseStrength = 1000.f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->ForceStrength = 1000.f;
	RadialForceComponent->SetupAttachment(GetRootComponent());
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
}

