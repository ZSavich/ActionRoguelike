// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SItemChest.h"

ASItemChest::ASItemChest()
{
 	PrimaryActorTick.bCanEverTick = true;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    SetRootComponent(BaseMesh);

    LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
    LidMesh->SetupAttachment(GetRootComponent());

    TargetRotation = 110.f;
}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
    LidMesh->SetRelativeRotation(FRotator(TargetRotation, 0.f, 0.f));
}

void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

