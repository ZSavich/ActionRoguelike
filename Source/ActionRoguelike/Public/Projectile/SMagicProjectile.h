// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SBaseProjectile.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
    float DamageAmount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tags")
    FGameplayTag ParryTagName;

public:	
	ASMagicProjectile();

protected:
    virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
    virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
