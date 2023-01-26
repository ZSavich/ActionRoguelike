// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SMagicProjectile.h"

void ASMagicProjectile::OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHitEvent(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	Destroy();
}
