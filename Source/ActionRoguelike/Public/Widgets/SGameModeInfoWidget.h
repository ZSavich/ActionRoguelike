// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameModeInfoWidget.generated.h"

class UTextBlock;
UCLASS(Blueprintable, BlueprintType, ClassGroup = UI)
class ACTIONROGUELIKE_API USGameModeInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, Meta = (BindWidget))
	UTextBlock* GameTimeText = nullptr;

	UPROPERTY(Transient)
	AGameStateBase* GameState;

	FTimerHandle TimerHandle_UpdateGameTime;

protected:
	virtual void NativeConstruct() override;

	/**/
	void HandleUpdateGameTime();
};
