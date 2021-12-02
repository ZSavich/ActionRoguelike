// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USAttributeComponent* AttributeComponent;
    
public:	
	ASTargetDummy();

protected:
    UFUNCTION()
    void OnActorHit(AActor* InstigatorActor, USAttributeComponent* AttributeComp, float CurrentHealth, float Delta);

};
