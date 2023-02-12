// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreditsChange, float ActualCredits);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	float Credits;
	
public:
	FOnCreditsChange OnCreditsChange;

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable)
	bool ApplyCreditChange(const float Amount);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCredits() const { return Credits; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CanAllow(const float NeedCredits) const { return Credits >= NeedCredits; }

	/* Console Command Functions */
	UFUNCTION(Exec)
	void SetCredits(const float Amount);
};
