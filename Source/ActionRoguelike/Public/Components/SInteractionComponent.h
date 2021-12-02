// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category="Interaction")
    float InteractionRadius;

    UPROPERTY(EditDefaultsOnly, Category="Debug")
    bool bDrawDebugInformation;

public:	
	USInteractionComponent();
    
    void PrimaryInteract() const;
};
