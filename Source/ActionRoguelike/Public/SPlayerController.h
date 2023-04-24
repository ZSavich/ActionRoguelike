// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> PauseMenuWidgetInstance;

public:
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

protected:
	bool IsPauseMenuValid();

private:
	bool bInPause = false;
	UUserWidget* CreatePauseWidget();
};
