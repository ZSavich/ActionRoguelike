// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/SProjectileBase.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	/** DashTeleport properties */
	UPROPERTY(EditAnywhere, Category = "Dash Properties")
	float AutoExplodeTime;
	
private:
	/** DashTeleport Timers */
	FTimerHandle TimerHandle_Teleport;
	FTimerHandle TimerHandle_AutoExplode;
	
public:
	ASDashProjectile();
	
	virtual void BeginPlay() override;
	virtual void OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
protected:
	void Explode();
	void Teleport();
};