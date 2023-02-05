// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, Category = "Adjusting")
	FVector VerticalOffset;

	/** Widget Components */
	UPROPERTY(Transient, Meta = (BindWidget))
	USizeBox* ParentSizeBox = nullptr;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAttachedActor(AActor* Actor);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
