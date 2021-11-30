// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Collision")
    USphereComponent* SphereComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UProjectileMovementComponent* MovementComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystemComponent* EffectComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* ExplosionEffect;
    
public:	
	ASMagicProjectile();

protected:
	virtual void BeginPlay() override;
};
