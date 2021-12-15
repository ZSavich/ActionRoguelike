// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actions")
    TArray<TSubclassOf<USAction>> ActionClasses;
    
    TArray<USAction*> Actions;
    
public:	
	USActionComponent();

protected:
	virtual void BeginPlay() override;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void AddAction(TSubclassOf<USAction> ActionClass);
    
    bool StartActionByName(AActor* InstigatorActor, const FName& ActionName);
    bool StopActionByName(AActor* InstigatorActor, const FName& ActionName);
};
