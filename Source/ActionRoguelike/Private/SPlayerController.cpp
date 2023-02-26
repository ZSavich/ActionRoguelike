// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "Widgets/SCreditsWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	// Create Pause Menu Widget
	
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::TogglePauseMenu()
{
	if (IsPauseMenuValid())
	{
		if (bIsPauseMenuVisible) // Hide the Pause Menu
		{
			// Hide the Pause Menu Widget
			PauseMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
			
			bIsPauseMenuVisible = false;
		}
		else // Show the Pause Menu
		{
			// Show the Pause Menu Widget and make enable to interact with it
			PauseMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
			
			bIsPauseMenuVisible = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Can't open PauseMenu."))
	}
}

bool ASPlayerController::IsPauseMenuValid()
{
	if (!PauseMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Need setup PauseMenuWidgetClass variable."))
		return nullptr;
	}

	if (!PauseMenuWidgetInstance)
	{
		// First call should create the PauseMenu widget
		PauseMenuWidgetInstance = CreateWidget(this, PauseMenuWidgetClass, "PauseMenuWidget");
		PauseMenuWidgetInstance->AddToViewport(2);
		PauseMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
	
	return IsValid(PauseMenuWidgetInstance);
}
