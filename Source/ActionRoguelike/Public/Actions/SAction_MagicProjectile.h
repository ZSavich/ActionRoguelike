// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_MagicProjectile.generated.h"

class ASProjectileBase;

UCLASS()
class ACTIONROGUELIKE_API USAction_MagicProjectile : public USAction
{
	GENERATED_BODY()

protected:
	/** General Properties */
	UPROPERTY(EditAnywhere, Category = "Projectile")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<ASProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float AttackDelay;

	FTimerHandle TimerHandle_SpawnProjectile;

public:
	USAction_MagicProjectile();

protected:
	virtual bool StartAction_Implementation(AActor* Instigator) override;
	
	void AttackDelayElapsed(ACharacter* InstigatorCharacter);
};
