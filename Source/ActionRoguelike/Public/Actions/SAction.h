// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class USActionComponent;

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tags")
    FGameplayTagContainer GrandsTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tags")
    FGameplayTagContainer BlockedTags;

    bool bIsRunning;

public:
    USAction();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ActionDescription")
    FName ActionName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tags")
    bool bAutoStart;

protected:
    UFUNCTION(BlueprintCallable)
    USActionComponent* GetOwningComponent() const;

public:
    virtual UWorld* GetWorld() const override;
    
    UFUNCTION(BlueprintNativeEvent)
    void StartAction(AActor* InstigatorActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void StopAction(AActor* InstigatorActor);

    UFUNCTION(BlueprintNativeEvent)
    bool CanStart();
    
    FORCEINLINE bool IsRunning() const { return bIsRunning; }
};
