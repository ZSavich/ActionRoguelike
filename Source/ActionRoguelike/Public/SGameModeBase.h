// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class ASAICharacter;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	/** Spawn Bots properties */
	UPROPERTY(EditAnywhere, Category = "Spawner")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ASAICharacter> BotClass;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnTimerInterval;

	FTimerHandle TimerHandle_SpawnBots;

public:
	ASGameModeBase();
	
	virtual void StartPlay() override;

protected:
	/** Spawn Bots functions */
	UFUNCTION()
	void HandleOnSpawnBotsFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBotTimerElapsed();
};
