// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SCreditsWidget.h"
#include "SPlayerState.h"
#include "Components/TextBlock.h"

void USCreditsWidget::BindWidgetComponents()
{
	if (ASPlayerState* PlayerState = GetOwningPlayer()->GetPlayerState<ASPlayerState>())
	{
		if (!PlayerState->OnCreditsChange.IsBound())
		{
			PlayerState->OnCreditsChange.AddUObject(this, &USCreditsWidget::UpdateCredits);	
		}
	}
}

void USCreditsWidget::UpdateCredits(float Credits) const
{
	if (CreditsCounterText)
	{
		CreditsCounterText->SetText(FText::AsNumber(Credits));
	}
}
