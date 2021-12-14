// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;
class UImage;

UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
    USizeBox* ParentSizeBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UMG")
    FVector2D WorldOffset;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    UPROPERTY()
    AActor* AttachedActor;

    UPROPERTY(meta = (BindWidget))
    UImage* HealthBarImage;
};
