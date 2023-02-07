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
	UPROPERTY(EditAnywhere, Category = "Properties|BotSpawner")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditAnywhere, Category = "Properties|BotSpawner")
	TSubclassOf<ASAICharacter> BotClass;

	UPROPERTY(EditAnywhere, Category = "Properties|BotSpawner")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditAnywhere, Category = "Properties|BotSpawner")
	float SpawnTimerInterval;

	UPROPERTY(EditAnywhere, Category = "Properties|Player")
	float RespawnPlayerDelay;

	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_RespawnPlayer;

public:
	ASGameModeBase();
	
	virtual void StartPlay() override;

	void OnActorKilled(AActor* VictimActor, AActor* InstigatorActor);
	
	/** Console Command Function */
	UFUNCTION(Exec)
	void KillAllBots() const;

protected:
	UFUNCTION()
	void RespawnPlayerElapsed(AActor* PlayerActor);
	
	/** Spawn Bots functions */
	UFUNCTION()
	void HandleOnSpawnBotsFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBotTimerElapsed();
};
