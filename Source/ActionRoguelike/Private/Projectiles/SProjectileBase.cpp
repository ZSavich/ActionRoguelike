// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SProjectileBase.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
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
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
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

	// Create a flight audio component
	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FlightAudio"));
	FlightAudioComponent->SetupAttachment(GetRootComponent());

	// Set default properties
	LifeTime = 5.f;
	Damage = 0.f;

	bActivateSpread = false;
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

	if (const ASCharacter* InstigatorSCharacter = GetInstigator<ASCharacter>())
	{
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, InstigatorSCharacter->GetMesh(), InstigatorSCharacter->HandSocketName);
	}

	// Spread functional
	if (bActivateSpread && ProjectileMovementComponent)
	{
		const FVector RandSpread = FVector(1.f, FMath::RandRange(0.9,1.1), 1.f);
		ProjectileMovementComponent->Velocity *= RandSpread;
	}

	SetLifeSpan(LifeTime);
}

void ASProjectileBase::OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetInstigator()) return;
	Explode();
}

void ASProjectileBase::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator()) return;
	
	Explode();
	USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);
}

void ASProjectileBase::Explode()
{
	ensure(IsValid(this));
	
	UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), 0.f, 500.f);
	
	Destroy();
}
