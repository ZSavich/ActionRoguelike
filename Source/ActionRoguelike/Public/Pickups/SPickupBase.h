// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SGameplayInterface.h"
#include "SPickupBase.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
	/** General Pickup Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereCollision;

	/** Sound Effects */
	UPROPERTY(EditAnywhere, Category = "Effects|Sounds")
	USoundBase* PickupSound;

	/** Visual Effects */
	UPROPERTY(EditAnywhere, Category = "Effects|VFX")
	UParticleSystem* PickupEffect;
	
	/** Pickup Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties|Spawn")
	bool bCanRespawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties|Spawn", Meta = (EditCondition = "bCanRespawn"))
	float RespawnTime;

	/** Pickup Timers */
	FTimerHandle TimerHandle_Respawn;
	
public:	
	ASPickupBase();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintCallable)
	virtual bool ActivatePickup(APawn* InstigatorPawn);
	
	virtual void Respawn();
	
};
