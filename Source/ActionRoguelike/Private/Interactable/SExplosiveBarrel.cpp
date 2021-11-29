// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
 	PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetCollisionProfileName("OverlapAllDynamic");
    MeshComp->SetupAttachment(GetRootComponent());

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
    RadialForceComp->Falloff = RIF_Linear;
    RadialForceComp->SetAutoActivate(false);
    RadialForceComp->Radius = 500.f;
    RadialForceComp->ImpulseStrength = 2000.f;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->SetupAttachment(GetRootComponent());
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ASExplosiveBarrel::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    RadialForceComp->FireImpulse();
}

