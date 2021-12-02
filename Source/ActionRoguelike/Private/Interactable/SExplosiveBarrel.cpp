// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
 	PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetupAttachment(GetRootComponent());

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
    RadialForceComp->Falloff = RIF_Linear;
    RadialForceComp->SetAutoActivate(false);
    RadialForceComp->Radius = 500.f;
    RadialForceComp->ImpulseStrength = 10000.f;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
    RadialForceComp->SetupAttachment(GetRootComponent());

    bDrawDebugInformation = false;
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}


void ASExplosiveBarrel::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
    FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    RadialForceComp->FireImpulse();

    if(bDrawDebugInformation)
    {
        UE_LOG(LogTemp, Display, TEXT("Barrel was hited by %s"), *Other->GetName());
        
        const auto DebugString = FString::Printf(TEXT("Hit Location - %s"), *Hit.ImpactPoint.ToCompactString());
        DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugString, nullptr, FColor::Blue, 2.f, true);
        DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComp->Radius, 32.f, FColor::Red, false, 2.f, 0.f, 1.f);
    }
}
