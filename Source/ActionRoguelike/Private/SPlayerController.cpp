// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "Widgets/SCreditsWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ensure(PauseMenuWidgetClass);
	
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::TogglePauseMenu()
{
	bInPause = !bInPause;
	SetPause(bInPause);
	if (IsPauseMenuValid())
	{
		PauseMenuWidgetInstance->SetVisibility(bInPause ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		SetShowMouseCursor(bInPause);
		if (bInPause)
		{
			SetInputMode(FInputModeUIOnly());
		}
		else
		{
			SetInputMode(FInputModeGameOnly());
		}
	}
	else
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Can't open PauseMenu."))
	}
}

bool ASPlayerController::IsPauseMenuValid()
{
	PauseMenuWidgetInstance = PauseMenuWidgetInstance ? PauseMenuWidgetInstance : CreatePauseWidget();
	return IsValid(PauseMenuWidgetInstance);
}

UUserWidget* ASPlayerController::CreatePauseWidget()
{
	UUserWidget* PauseWidget = CreateWidget(this, PauseMenuWidgetClass, "PauseMenuWidget");
	PauseWidget->AddToViewport(2);
	PauseWidget->SetVisibility(ESlateVisibility::Hidden);
	return PauseWidget;
}
