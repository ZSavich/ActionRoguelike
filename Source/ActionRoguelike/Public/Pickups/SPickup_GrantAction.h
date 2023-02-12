// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SPickup_GrantAction.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API ASPickup_GrantAction : public ASPickupBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<USAction> GrantAction;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
