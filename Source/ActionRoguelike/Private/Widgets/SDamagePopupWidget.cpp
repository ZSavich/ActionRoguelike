// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SDamagePopupWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void USDamagePopupWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ShowDamageAnim)
	{
		// Remove the widget after animation finished
		EndDelegate.BindDynamic(this, &USDamagePopupWidget::AnimationFinished);
		BindToAnimationFinished(ShowDamageAnim, EndDelegate);
	}
}

void USDamagePopupWidget::SetDamageText(float Damage)
{
	DamageText->SetText(FText::AsNumber(Damage));

	PlayAnimationForward(ShowDamageAnim, 1.f, true);
}

void USDamagePopupWidget::AnimationFinished()
{
	//RemoveFromParent();
}
