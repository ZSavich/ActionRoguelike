// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static bool ApplyDamage(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable)
    static bool ApplyDirectionalDamage(UPrimitiveComponent* Component, AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, const FHitResult& HitResult);
};
