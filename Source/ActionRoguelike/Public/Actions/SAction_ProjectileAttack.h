// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class ASBaseProjectile;

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:
    USAction_ProjectileAttack();
    
protected:
    UPROPERTY(EditDefaultsOnly, Category="Projectiles")
    TSubclassOf<ASBaseProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category="Animations")
    UAnimMontage* ProjectileAttackMontage;
    
    FTimerHandle TimerHandle_ProjectileAttack;

    UPROPERTY(EditDefaultsOnly, Category="Sockets")
    FName MuzzleSocketName;

    virtual void StartAction_Implementation(AActor* InstigatorActor) override;
    
    void ProjectileAttack_TimeElapsed(ACharacter* InstigatorCharacter);
};
