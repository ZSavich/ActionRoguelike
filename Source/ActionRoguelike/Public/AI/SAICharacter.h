// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UPawnSensingComponent* PawnSensingComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USAttributeComponent* AttributeComp;
    
public:
    ASAICharacter();

protected:
    UFUNCTION()
    void OnDeadHandle(AActor* InstigatorActor, AActor* VictimActor);
    
public:
    virtual void PostInitializeComponents() override;
    
    UFUNCTION()
    void OnPawnSeen(APawn* TargetPawn);
};
