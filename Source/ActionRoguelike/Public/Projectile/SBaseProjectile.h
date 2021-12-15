// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

// Abstract - Marks that this class can't be spawned in the world
UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBaseProjectile : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Collision")
    USphereComponent* SphereComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UProjectileMovementComponent* MovementComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    TSubclassOf<UCameraShakeBase> CameraShake;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystemComponent* EffectComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* MuzzleFlashEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
    UAudioComponent* FlightSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
    USoundBase* ImpactSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
    float LifeSpan;
    
public:	
	ASBaseProjectile();

    FName MuzzleSocketName;

protected:
	virtual void BeginPlay() override;

    virtual void SpawnImpactEffects();
    
    UFUNCTION()
    virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
