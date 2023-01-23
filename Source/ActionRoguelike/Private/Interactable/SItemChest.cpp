// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/SItemChest.h"

ASItemChest::ASItemChest()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(GetRootComponent());

	TargetPitch = 110.f;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (LidMesh)
	{
		LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
	}
}
