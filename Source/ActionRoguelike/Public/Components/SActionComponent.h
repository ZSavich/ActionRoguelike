// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

    UPROPERTY(Replicated, VisibleAnywhere)
    TArray<USAction*> Actions;
    
public:	
	USActionComponent();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tags")
    FGameplayTagContainer ActiveGameplayTags;

protected:
	virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerStartAction(AActor* InstigatorActor, FName Action);

    UFUNCTION(Server, Reliable)
    void ServerStopAction(AActor* InstigatorActor, FName Action);
    
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void AddAction(AActor* InstigatorActor, TSubclassOf<USAction> ActionClass);
    UFUNCTION(BlueprintCallable)
    void RemoveAction(USAction* Action);

    UFUNCTION(BlueprintCallable)
    bool StartActionByName(AActor* InstigatorActor, const FName& ActionName);
    UFUNCTION(BlueprintCallable)
    bool StopActionByName(AActor* InstigatorActor, const FName& ActionName);

    virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
