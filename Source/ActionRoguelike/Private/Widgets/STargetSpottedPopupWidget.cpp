// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/STargetSpottedPopupWidget.h"

USTargetSpottedPopupWidget::USTargetSpottedPopupWidget()
{
	AutoDestroyTime = 3.f;
}

void USTargetSpottedPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Auto Remove the widget after a certain time
	FTimerHandle TimerHandle_AutoRemove;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutoRemove, this, &UUserWidget::RemoveFromParent, AutoDestroyTime, false);
}
