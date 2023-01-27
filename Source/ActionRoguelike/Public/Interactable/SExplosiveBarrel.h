// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SExplosiveBarrel.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageRadius;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BarrelMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
public:	
	ASExplosiveBarrel();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
