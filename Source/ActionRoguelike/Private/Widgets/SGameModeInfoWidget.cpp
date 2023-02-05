// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SGameModeInfoWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"

void USGameModeInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UWorld* World = GetWorld())
	{
		GameState = World->GetGameState();
		if (GameState)
		{
			World->GetTimerManager().SetTimer(TimerHandle_UpdateGameTime, this, &USGameModeInfoWidget::HandleUpdateGameTime, .5, true);
		}
	}
}

void USGameModeInfoWidget::HandleUpdateGameTime()
{
	GameState = GameState ? GameState : GetWorld()->GetGameState();
	if (GameState && GameTimeText)
	{
		const float TimeSeconds = GameState->GetServerWorldTimeSeconds();
		
		const int32 NumMinutes = FMath::FloorToInt(TimeSeconds/60.f);
		const int32 NumSeconds = FMath::FloorToInt(TimeSeconds-(NumMinutes*60.f));
		
		const FString GameTimeString = FString::Printf(TEXT("%02d:%02d"), NumMinutes, NumSeconds);
		GameTimeText->SetText(FText::FromString(GameTimeString));
	}
}
