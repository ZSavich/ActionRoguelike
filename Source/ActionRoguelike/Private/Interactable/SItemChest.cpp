// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SItemChest.h"

#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
 	PrimaryActorTick.bCanEverTick = true;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    SetRootComponent(BaseMesh);

    LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
    LidMesh->SetupAttachment(GetRootComponent());

    TargetRotation = 110.f;
    SetReplicates(true);
}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
    bLidOpened = !bLidOpened;
    OnRep_LidOpened();
}


void ASItemChest::OnRep_LidOpened()
{
    const float TogglePitch = bLidOpened ? TargetRotation : 0.f;
    LidMesh->SetRelativeRotation(FRotator(TogglePitch, 0.f, 0.f));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASItemChest, bLidOpened);
}

