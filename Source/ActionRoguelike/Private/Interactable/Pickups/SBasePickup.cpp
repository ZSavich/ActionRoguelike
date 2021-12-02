// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pickups/SBasePickup.h"

#include "SCharacter.h"

ASBasePickup::ASBasePickup()
{
 	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SetRootComponent(Mesh);

    InactivateTime = 5.f;
}

void ASBasePickup::Activate(const APawn* InstigatorActor)
{
    if(GetWorldTimerManager().IsTimerActive(TimerHandle_Respawn)) return;
    if(!Effect(InstigatorActor)) return;
    
    SetActorHiddenInGame(true);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASBasePickup::Respawn, InactivateTime);
}

void ASBasePickup::Respawn()
{
    SetActorHiddenInGame(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASBasePickup::Interact_Implementation(APawn* InstigatorPawn)
{
    ISGameplayInterface::Interact_Implementation(InstigatorPawn);
    Activate(InstigatorPawn);
}

