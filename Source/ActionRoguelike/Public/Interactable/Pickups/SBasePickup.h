// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SBasePickup.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBasePickup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
    float InactivateTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
    float CreditsForInteract;

    FTimerHandle TimerHandle_Respawn;
    
public:	
	ASBasePickup();

protected:    
    UFUNCTION(BlueprintCallable)
    virtual void Activate(AActor* InstigatorActor);
    virtual bool Effect(AActor* InstigatorActor) {return false;}
    virtual void Respawn();

    virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
