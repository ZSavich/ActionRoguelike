// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionEffect.h"
#include "SActionBuff.generated.h"

class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API USActionBuff : public USActionEffect
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
    float ReflectedDamage;
    
public:
    USActionBuff();

    UFUNCTION()
    void OnHealthChangedHandle(AActor* InstigatorActor, USAttributeComponent* AttributeComp, float CurrentHealth, float Delta);

    
    virtual void StartAction_Implementation(AActor* InstigatorActor) override;
    virtual void StopAction_Implementation(AActor* InstigatorActor) override;
};
