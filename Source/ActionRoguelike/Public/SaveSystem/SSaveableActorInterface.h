// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSaveableActorInterface.generated.h"

/* Interface that marks Actors that will be saved and loaded */
UINTERFACE(MinimalAPI)
class USSaveableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class ACTIONROGUELIKE_API ISSaveableActorInterface
{
	GENERATED_BODY()

public:
	/* Event that will trigger when data is loaded */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Savable Actor")
	void ActorSaveDataLoaded();

	/* Event that will trigger when data is saved */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Savable Actor")
	void ActorSaveDataSaved();
};
