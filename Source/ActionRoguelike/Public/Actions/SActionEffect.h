// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SActionEffect.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
    float Duration;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
    float Period;

    FTimerHandle TimerHandle_Duration;
    FTimerHandle TimerHandle_Period;
    
public:
    USActionEffect();

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void ExecutePeriodEffect(AActor* InstigatorActor);

public:
    virtual void StartAction_Implementation(AActor* InstigatorActor) override;
    virtual void StopAction_Implementation(AActor* InstigatorActor) override;
	
};
