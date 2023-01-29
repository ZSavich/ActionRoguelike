// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SDamagePopupWidget.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USDamagePopupWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/** Components */
	UPROPERTY(Transient, Meta = (BindWidget))
	UTextBlock* DamageText = nullptr;

	/** Animations */
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* ShowDamageAnim = nullptr;

	FWidgetAnimationDynamicEvent EndDelegate;
	
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void SetDamageText(float Damage);

private:
	UFUNCTION()
	void AnimationFinished();
};
