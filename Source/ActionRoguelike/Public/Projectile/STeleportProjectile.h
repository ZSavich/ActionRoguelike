// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/SBaseProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

protected:
    FTimerHandle TimerHandle_Explodes;
    FTimerHandle TimerHandle_Teleport;
    
public:
    ASTeleportProjectile();

protected:
    UFUNCTION(BlueprintCallable)
    void Explodes();
    void Teleport();
    
public:
    virtual void BeginPlay() override;
};
