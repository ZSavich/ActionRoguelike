// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreditsScreen.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, const float, Credits);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCreditsChanged OnCreditsChanged;
    
    UFUNCTION(BlueprintCallable)
    FORCEINLINE float GetCredits() const { return Credits; }
    
    void AddCredits(const float Amount);
    
    FORCEINLINE bool CheckEnoughCredits(const float Amount) const { return Credits + Amount >= 0.f;};

private:
    float Credits = 0;
};
