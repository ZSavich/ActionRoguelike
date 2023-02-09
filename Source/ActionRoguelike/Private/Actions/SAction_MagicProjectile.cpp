// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SAction_MagicProjectile.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Projectiles/SProjectileBase.h"

USAction_MagicProjectile::USAction_MagicProjectile()
{
	AttackDelay = 0.2f;
	HandSocketName = FName("Muzzle_01");
}

bool USAction_MagicProjectile::StartAction_Implementation(AActor* Instigator)
{
	if (Super::StartAction_Implementation(Instigator))
	{
		if (ACharacter* InstCharacter = Cast<ACharacter>(Instigator))
		{
			InstCharacter->PlayAnimMontage(AttackMontage);
			const FTimerDelegate SpawnProjectileDelegate = FTimerDelegate::CreateUObject(this, &USAction_MagicProjectile::AttackDelayElapsed, InstCharacter);

			if (const UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(TimerHandle_SpawnProjectile, SpawnProjectileDelegate, AttackDelay , false);
				return true;
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("EUD::Can't spawn projectile in %s"), *GetNameSafe(this));
	return false;
}

void USAction_MagicProjectile::AttackDelayElapsed(ACharacter* InstigatorCharacter)
{
	if (TimerHandle_SpawnProjectile.IsValid())
	{
		TimerHandle_SpawnProjectile.Invalidate();
	}
	
	if (InstigatorCharacter)
	{
		USkeletalMeshComponent* Mesh = InstigatorCharacter->GetMesh();
		UCameraComponent* FollowCamera = InstigatorCharacter->FindComponentByClass<UCameraComponent>();
		UWorld* World = GetWorld();

		if (Mesh && FollowCamera && World)
		{
			// Find Rotation of a target
			FHitResult HitResult;
	
			const FVector StartTrace = FollowCamera->GetComponentLocation();
			const FVector EndTrace = StartTrace + (FollowCamera->GetComponentRotation().Vector() * 50000);
			World->LineTraceSingleByChannel(
				HitResult,
				StartTrace,
				EndTrace,
				ECC_Visibility);

			const FVector TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;

			// Spawn Projectile
			const FVector SpawnLocation = Mesh->GetSocketLocation(HandSocketName); // Get Location of the character's muzzle socket
			const FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetPoint - SpawnLocation).Rotator(); // Get Rotation to the Target 
	
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = InstigatorCharacter;
			SpawnParams.Instigator = InstigatorCharacter;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
			World->SpawnActor<ASProjectileBase>(ProjectileClass, SpawnLocation, TargetRotation, SpawnParams);
		}
	}

	StopAction(InstigatorCharacter);
}
