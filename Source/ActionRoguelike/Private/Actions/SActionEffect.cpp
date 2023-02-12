// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SActionEffect.h"

USActionEffect::USActionEffect()
{
	Duration = 3.f;
	Period = 1.f;
	bAutoStart = true;
}

bool USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	if (!Super::StartAction_Implementation(Instigator)) return false;

	if (const UWorld* World = GetWorld())
	{
		if (Duration > 0.f)
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, "StopAction", Instigator);
			World->GetTimerManager().SetTimer(TimerHandle_Duration, TimerDelegate, Duration, false);
		}

		if (Period > 0.f)
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);
			World->GetTimerManager().SetTimer(TimerHandle_Period, TimerDelegate, Period, true);
		}
	}
	
	return true;
}

bool USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (!Super::StopAction_Implementation(Instigator)) return false;

	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_Duration);
		World->GetTimerManager().ClearTimer(TimerHandle_Period);
	}

	if (USActionComponent* ActionComponent = GetOwningComponent())
	{
		ActionComponent->RemoveAction(this);
	}

	return true;
}

void USActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
}
