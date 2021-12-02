// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/STargetDummy.h"
#include "SAttributeComponent.h"

ASTargetDummy::ASTargetDummy()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(MeshComp);

    AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
    AttributeComponent->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnActorHit);
}

void ASTargetDummy::OnActorHit(AActor* InstigatorActor, USAttributeComponent* AttributeComp, float CurrentHealth, float Delta)
{
    if(Delta < 0.f)
    {
        MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
    }
}
