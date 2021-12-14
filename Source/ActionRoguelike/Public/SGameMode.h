// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAICharacter.h"
#include "SBasePickup.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

class UEnvQuery;
struct FEnvQueryResult;

UCLASS()
class ACTIONROGUELIKE_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemies")
    TSubclassOf<ASAICharacter> MinionClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemies")
    UEnvQuery* SpawnBotQuery;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemies")
    UCurveFloat* DifficultyCurve;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemies")
    float SpawnTimerInterval;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickups")
    TMap<TSubclassOf<ASBasePickup>, uint8> Pickups;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickups")
    UEnvQuery* SpawnPickupQuery;
    
    
    FTimerHandle TimerHandle_SpawnBots;

public:
    ASGameMode();

protected:
    void SpawnBotsTimerElapsed();
    void OnSpawnBotQueryCompleted(const TSharedPtr<FEnvQueryResult> QueryResult) const;
    void OnSpawnPickupQueryCompleted(const TSharedPtr<FEnvQueryResult> QueryResult) const;
    void RespawnPlayer(AController* Controller);
    void SpawnPickups();

public:
    virtual void StartPlay() override;

    UFUNCTION(Exec)
    void KillAll();

    void OnActorKilled(AActor* VictimActor, AActor* InstigatorActor);
};
