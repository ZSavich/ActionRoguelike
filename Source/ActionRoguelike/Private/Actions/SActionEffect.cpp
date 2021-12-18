// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionEffect.h"

#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
    bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    if(Duration > 0.f)
    {
        FTimerDelegate TimerDelegate_Duration;
        TimerDelegate_Duration.BindUObject(this, &USActionEffect::StopAction_Implementation, InstigatorActor);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, TimerDelegate_Duration, Duration, false);
    }

    if(Period > 0.f)
    {
        FTimerDelegate TimerDelegate_Period;
        TimerDelegate_Period.BindUObject(this, &USActionEffect::ExecutePeriodEffect, InstigatorActor);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, TimerDelegate_Period, Period, true, 0.f);
    }
}

void USActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
    if(GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
        ExecutePeriodEffect(InstigatorActor);
    
    Super::StopAction_Implementation(InstigatorActor);

    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);

    GetOwningComponent()->RemoveAction(this);
}