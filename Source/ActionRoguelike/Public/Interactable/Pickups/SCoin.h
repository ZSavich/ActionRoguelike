// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Pickups/SBasePickup.h"
#include "SCoin.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASBasePickup
{
	GENERATED_BODY()

public:
    ASCoin();

protected:
    virtual bool Effect(AActor* InstigatorActor) override;
};
