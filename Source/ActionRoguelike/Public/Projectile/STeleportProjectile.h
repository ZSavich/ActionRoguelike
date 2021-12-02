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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Teleport")
    float ExplosiveDelay;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Teleport")
    float TeleportDelay;
    
    FTimerHandle TimerHandle_Explodes;
    FTimerHandle TimerHandle_Teleport;
    
public:
    ASTeleportProjectile();

protected:
    UFUNCTION(BlueprintCallable)
    void Explodes();
    void Teleport();

    virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    
public:
    virtual void BeginPlay() override;
};
