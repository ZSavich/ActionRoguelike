// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SaveSystem/SSaveGame.h"
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

	/* Save Game Data */
	TObjectPtr<USSaveGame> CurrentSaveData;
	FString CurrentSlotName;

public:
	ASGameModeBase();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

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

	/* Save Game Data Functions */
	UFUNCTION(BlueprintCallable)
	void WriteSaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadSaveGame();
};
