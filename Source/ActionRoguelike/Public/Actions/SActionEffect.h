// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SActionEffect.generated.h"

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

protected:
	/** Effect's Properties*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float Period;

private:
	/** Effect's Timers */
	FTimerHandle TimerHandle_Duration;
	FTimerHandle TimerHandle_Period;

	UPROPERTY(Replicated)
	float TimeStarted;

public:
	USActionEffect();

	virtual bool StartAction_Implementation(AActor* Instigator) override;
	virtual bool StopAction_Implementation(AActor* Instigator) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void ExecutePeriodEffect(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;
	
	
};
