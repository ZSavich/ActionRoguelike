// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGunTurret.generated.h"

class ASCharacter;
class ASBaseProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASGunTurret : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
    TSubclassOf<ASBaseProjectile> ProjectileClass;

    // Static Mesh implemented into the Blueprint
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
    int32 AmountOfAttacksInRow;
    
    UPROPERTY()
    ASCharacter* TargetPawn;

    FTimerHandle TimerHandle_LocateTarget;
    FTimerHandle TimerHandle_AttackTarget;

    int32 CountOfAttacks;
    
public:
    ASGunTurret();

protected:
	virtual void BeginPlay() override;

    void LocateTarget();
    void FindAndAttackTarget();

};
