// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UPawnSensingComponent* PawnSensingComp;
    
public:
    ASAICharacter();

    UFUNCTION()
    void OnPawnSeen(APawn* TargetPawn);

    virtual void PostInitializeComponents() override;
};
