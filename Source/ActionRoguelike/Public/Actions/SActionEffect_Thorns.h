// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

class USAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	USActionEffect_Thorns();

	virtual bool StartAction_Implementation(AActor* Instigator) override;
	virtual bool StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	void ExecuteThornEffect(AActor* Instigator, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
