// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameplayFunctionLibrary.h"
#include "Components/SAttributeComponent.h"

static TAutoConsoleVariable<int> CVarHitImpulseStrength(TEXT("su.HitImpulseStrength"), 300000.f, TEXT("The strength of the impulse that throws an enemy when it gets hit."));

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor))
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			const int32 HitImpulse = CVarHitImpulseStrength.GetValueOnGameThread();
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitComponent->AddImpulseAtLocation(Direction * HitImpulse, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
