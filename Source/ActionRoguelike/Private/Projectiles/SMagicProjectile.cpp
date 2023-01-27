// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SMagicProjectile.h"

ASMagicProjectile::ASMagicProjectile()
{
	Damage = 25.f;
}

void ASMagicProjectile::OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHitEvent(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	Destroy();
}
