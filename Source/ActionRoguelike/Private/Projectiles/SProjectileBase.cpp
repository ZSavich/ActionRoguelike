// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SProjectileBase.h"

#include "Components/SAttributeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASProjectileBase::ASProjectileBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// Create a sphere component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);
	
	// Create a particle system component
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(GetRootComponent());

	// Create a projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f; // The Projectile should move forward 
	ProjectileMovementComponent->InitialSpeed = 4000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // Match direction of the projectile's velocity
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;

	// Set default properties
	LifeTime = 5.f;
	Damage = 0.f;
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (SphereComponent)
	{
		SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
		SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnHitEvent);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnBeginOverlapEvent);
	}

	SetLifeSpan(LifeTime);
}

void ASProjectileBase::OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner()) return;
	
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, Hit.ImpactPoint);
	}
}

void ASProjectileBase::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;
	
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, SweepResult.ImpactPoint);
	}

	if (Damage > 0.f)
	{
		if (USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			AttributeComponent->ApplyHealthChange(this, -Damage);
			Destroy();
		}
	}
}
