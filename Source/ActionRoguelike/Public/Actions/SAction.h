// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"


UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ActionDescription")
    FName ActionName;
    
    UFUNCTION(BlueprintNativeEvent)
    void StartAction(AActor* InstigatorActor);

    UFUNCTION(BlueprintNativeEvent)
    void StopAction(AActor* InstigatorActor);

    virtual UWorld* GetWorld() const override;
};
