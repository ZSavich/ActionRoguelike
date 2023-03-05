// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickupBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Properties", Meta = (AllowPrivateAccess = "true"))
	float HealValue;
	
public:
	ASHealthPotion();
	
	virtual bool ActivatePickup(APawn* InstigatorPawn) override;

	/** Gameplay Interface's Functions */
	virtual FText GetInteractText(APawn* InstigatorPawn) override;
};
