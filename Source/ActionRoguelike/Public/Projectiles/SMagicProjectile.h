// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()

private:
	/** Default Projectile's Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	/** Visual Effects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> EffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitEffect;
	
public:	
	ASMagicProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
