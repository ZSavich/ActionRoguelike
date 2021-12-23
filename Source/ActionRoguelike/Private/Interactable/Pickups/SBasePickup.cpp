// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pickups/SBasePickup.h"

#include "SCharacter.h"
#include "SPlayerState.h"

ASBasePickup::ASBasePickup()
{
 	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SetRootComponent(Mesh);

    SetReplicates(true);

    InactivateTime = 5.f;
    CreditsForInteract = 0;
}

void ASBasePickup::Activate(AActor* InstigatorActor)
{
    if(GetWorldTimerManager().IsTimerActive(TimerHandle_Respawn)) return;

    const auto InstigatorPawn = Cast<APawn>(InstigatorActor);
    
    const auto PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
    if(!PS) return;
    const auto bIsEnough = PS->CheckEnoughCredits(CreditsForInteract);
    if(!bIsEnough) return;
    
    if(!Effect(InstigatorActor)) return;
    PS->AddCredits(CreditsForInteract);
    
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

