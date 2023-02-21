// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCreditsWidget.generated.h"

class UTextBlock;

UCLASS(Blueprintable, BlueprintType, ClassGroup = UI)
class ACTIONROGUELIKE_API USCreditsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, Meta = (BindWidget))
	UTextBlock* CreditsCounterText = nullptr;

protected:
	UFUNCTION(BlueprintCallable)
	void BindWidgetComponents();

	void UpdateCredits(float Credits) const;
};
