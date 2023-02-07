// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SCoinPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCoinPickup : public ASPickupBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Properties")
	float GrantCredits;

public:
	ASCoinPickup();

	virtual bool ActivatePickup(APawn* InstigatorPawn) override;
};
