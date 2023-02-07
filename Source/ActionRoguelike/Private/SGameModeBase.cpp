// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/SAICharacter.h"
#include "Components/SAttributeComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
	RespawnPlayerDelay = 5.f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* InstigatorActor)
{
	if (VictimActor->IsA(ASCharacter::StaticClass()))
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, TEXT("RespawnPlayerElapsed"), VictimActor);
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnPlayer, TimerDelegate, RespawnPlayerDelay, false);
	}
}

void ASGameModeBase::KillAllBots() const
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		if (USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(*It))
		{
			Attributes->KillSelf();
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AActor* PlayerActor)
{
	if (APlayerController* Controller = PlayerActor->GetInstigatorController<APlayerController>())
	{
		Controller->UnPossess();
		RestartPlayer(Controller);

		if (IsValid(PlayerActor))
		{
			PlayerActor->Destroy();
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	const bool bCanSpawnBots = CVarSpawnBots.GetValueOnGameThread();
	if (!bCanSpawnBots)
	{
		return;
	}
	
	if (!DifficultyCurve)
	{
		UE_LOG(LogRogueAI, Error, TEXT("EUD::DifficultyCurve is nullptr. Please set this for current game mode!"));
		return;
	}

	// Count live bots
	int32 NumOfAliveBots = 0;
	for (TObjectIterator<ASAICharacter> It; It; ++It)
	{
		if (ASAICharacter* AICharacter = *It)
		{
			if (USAttributeComponent::IsActorAlive(AICharacter))
			{
				NumOfAliveBots++;
			}
		}
	}

	if (NumOfAliveBots >= DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds))
	{
		// We have enough live bots.
		return;
	}

	// Find location for bot spawners
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::HandleOnSpawnBotsFinished);
	}
}

void ASGameModeBase::HandleOnSpawnBotsFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	QueryInstance->GetOnQueryFinishedEvent().Clear();
	
	if (QueryStatus != EEnvQueryStatus::Success || !BotClass)
	{
		UE_LOG(LogRogueAI, Warning, TEXT("EUD::SpawnBotQueue failed! We can't spawn new bots because of it."));
		return;
	}

	UWorld* World = GetWorld();
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (World && SpawnLocations.IsValidIndex(0))
	{
		World->SpawnActor<ASAICharacter>(BotClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}
