// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/SItemChest.h"

#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(GetRootComponent());

	// Replication
	SetReplicates(true);
}

void ASItemChest::BeginPlay()
{
	Super::BeginPlay();

	// Replication
	SetReplicateMovement(true);
}

void ASItemChest::ActorSaveDataLoaded_Implementation()
{
	ISSaveableActorInterface::ActorSaveDataLoaded_Implementation();
	OnRep_LidOpened();
}

FText ASItemChest::GetInteractText(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASItemChest::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}
