// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Pickups/SBasePickup.h"
#include "SHealthPotion.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASBasePickup
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Potion")
    float HealCount;
    
public:
    ASHealthPotion();
    
protected:
    virtual bool Effect(AActor* InstigatorActor) override;
};
