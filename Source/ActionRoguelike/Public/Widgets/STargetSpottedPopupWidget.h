// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWorldUserWidget.h"
#include "STargetSpottedPopupWidget.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USTargetSpottedPopupWidget : public USWorldUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float AutoDestroyTime;
	
public:
	USTargetSpottedPopupWidget();
	
	virtual void NativeConstruct() override;
};
