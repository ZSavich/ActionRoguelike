// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SPickupBase.h"

#include "Kismet/GameplayStatics.h"

ASPickupBase::ASPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Create a base mesh component
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Block);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetRootComponent(BaseMesh);

	// Create a collision sphere component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	SphereCollision->SetupAttachment(GetRootComponent());

	// General Properties
	bCanRespawn = false;
	RespawnTime = 10.f;
}

void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	
	if (ActivatePickup(InstigatorPawn))
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}

		if (PickupEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, PickupEffect, GetActorLocation());
		}

		// If pick-up can respawn we should hide the mesh and disable collision or destroy it
		if (bCanRespawn)
		{
			SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BaseMesh->SetVisibility(false);
			
			GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPickupBase::Respawn, RespawnTime, false);
		}
		else
		{
			Destroy();
		}
	}
}

void ASPickupBase::Respawn()
{
	if (TimerHandle_Respawn.IsValid())
	{
		TimerHandle_Respawn.Invalidate();
	}
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseMesh->SetVisibility(true);
}

bool ASPickupBase::ActivatePickup(APawn* InstigatorPawn)
{
	return true;
}