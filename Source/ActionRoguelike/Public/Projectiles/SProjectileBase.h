// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	/** Default Projectile's Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> FlightAudioComponent;

	/** Visual Effects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystemComponent> EffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	/** Sound Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	USoundBase* ImpactSound; 

	/** Base properties */
	UPROPERTY(EditAnywhere, Category = "Properties")
	float LifeTime;

	UPROPERTY(EditAnywhere, Category = "Prorepties")
	float Damage;
	
	UPROPERTY(EditAnywhere, Category = "Properties|Spread")
	bool bActivateSpread;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer ParryTags;
	
public:	
	ASProjectileBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void Explode();
	
	UFUNCTION()
	virtual void OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
