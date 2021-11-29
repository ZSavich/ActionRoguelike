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
    UPROPERTY(EditDefaultsOnly, Category="Components")
    USphereComponent* SphereComp;

    UPROPERTY(EditDefaultsOnly, Category="Components")
    UProjectileMovementComponent* MovementComp;

    UPROPERTY(EditDefaultsOnly, Category="Components")
    UParticleSystemComponent* EffectComp;
    
public:	
	ASMagicProjectile();

protected:
	virtual void BeginPlay() override;
};
