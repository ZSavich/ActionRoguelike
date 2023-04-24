// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/SAICharacter.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SSaveableActorInterface.h"
#include "SaveSystem/SSaveGame.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
	RespawnPlayerDelay = 5.f;

	/* Save Game Data */
	CurrentSlotName = "SaveGameData";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Optional slot name (Fall back to slot specified in SaveGameSettings class/INI otherwise)
	const FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	LoadSaveGame(); // LoadSaveGame(SelectedSaveSlot);
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Calling Before Super:: so we set variables before "BeginPlay" state is called in PlayerController (which is where we instantiate UI)
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (ensure(PlayerState))
	{
		PlayerState->LoadPlayerState(CurrentSaveData);
	}
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// Now we're ready override spawn location
	// Alternatively we could override code spawn location to use store locations immediately (skipping the whole 'find player start' logic)
	if (PlayerState)
	{
		PlayerState->OverrideSpawnTransform(CurrentSaveData);
	}
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
	
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogRogueAI, Warning, TEXT("EUD::SpawnBotQueue failed! We can't spawn new bots because of it."));
		return;
	}
	
	UWorld* World = GetWorld();
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (World && SpawnLocations.IsValidIndex(0))
	{
		TArray<FBotInfoRow*> BotInfoRows;
		BotsTable->GetAllRows("", BotInfoRows);

		const uint8 RandomNum = FMath::RandRange(0, BotInfoRows.Num() - 1);
		const FPrimaryAssetId& SelectedBotData = BotInfoRows[RandomNum]->BotDataAsset;
		if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
		{
			const TArray<FName> Bundles;
			const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnBotLoaded, SelectedBotData, SpawnLocations[0]);

			AssetManager->LoadPrimaryAsset(SelectedBotData, Bundles, Delegate);
		}
	}
}

void ASGameModeBase::WriteSaveGame()
{
	UE_LOG(LogTemp, Log, TEXT("EUD::Starting save data..."));
	// Clear arrays, may contain data from previous loaded SaveGame
	CurrentSaveData->SavedActors.Empty();
	CurrentSaveData->SavedPlayers.Empty();

	// Find actors with SaveablesActorInterface and write them into array
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USSaveableActorInterface::StaticClass(), Actors);

	// Iterate the array of actors with SaveableActorInterface
	for (AActor* Actor: Actors)
	{
		// Skip actors that are being destroyed
		if (Actor->IsPendingKillPending()) continue;

		// Record Actor's data
		FActorSaveData ActorRecord {*GetNameSafe(Actor), Actor->GetTransform()};

		// Pass the array to fill with data from Actor
		FMemoryWriter MemoryWriter(ActorRecord.ActorData);
		FSaveGameArchive Ar(MemoryWriter);
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveData->SavedActors.Add(ActorRecord);
	}

	// Iterate all player states, we don't have proper ID to match yet (requires Steam of EOS)
	if (AGameStateBase* GS = GetWorld()->GetGameState())
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(GS->PlayerArray[0]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveData);
			UE_LOG(LogTemp, Log, TEXT("EUD::Saved Player State in Game Mode."));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("EUD::Can't save Player State in Game Mode."));
		}
	}

	const bool bIsSaved = UGameplayStatics::SaveGameToSlot(CurrentSaveData, CurrentSlotName, 0);
	UE_LOG(LogTemp, Log, TEXT("EUD::SaveGame Data Result = %s"), bIsSaved ? TEXT("Successful") : TEXT("Failed"));
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0) == false)
	{
		CurrentSaveData = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("EUD::Created New SaveGame Data."))
		return;
	}

	CurrentSaveData = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
	if (CurrentSaveData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Failed to load SaveGame Data."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

	// Find Actors that have SaveableActorInterface
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USSaveableActorInterface::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		const FActorSaveData* FoundedActor = CurrentSaveData->SavedActors.FindByPredicate([&](const FActorSaveData& ActorData) { return *GetNameSafe(Actor) == ActorData.ActorName; });
		if (FoundedActor)
		{
			Actor->SetActorTransform(FoundedActor->ActorTransform);
			
			FMemoryReader FromBinary = FMemoryReader(FoundedActor->ActorData);
			FSaveGameArchive Ar(FromBinary);
			// Convert Binary array back into Actor's variables
			Actor->Serialize(Ar);

			// Call SaveableActorInterface's function that will update actor's state
			ISSaveableActorInterface::Execute_ActorSaveDataLoaded(Actor);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Successful loaded SaveGame Data."));
}

void ASGameModeBase::OnBotLoaded(FPrimaryAssetId LoadedPrimaryAssetId, FVector SpawnLocation)
{
	if (const UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		const USMonsterDataAsset* MonsterData = AssetManager->GetPrimaryAssetObject<USMonsterDataAsset>(LoadedPrimaryAssetId);

		if (!MonsterData)
		{
			UE_LOG(LogTemp, Warning, TEXT("EUD::Can't load MonsterData"));
			return;
		}
		
		ASAICharacter* SpawnedBot = GetWorld()->SpawnActor<ASAICharacter>(MonsterData->BotClass, SpawnLocation, FRotator::ZeroRotator);

		if (SpawnedBot)
		{
			UE_LOG(LogTemp, Log, TEXT("EUD::Spawned enemy: %s (%s)"), *GetNameSafe(SpawnedBot), *GetNameSafe(MonsterData));
				
			if (USActionComponent* ActionComponent = USActionComponent::GetActionComponent(SpawnedBot))
			{
				for (const TSubclassOf<USAction>& Action : MonsterData->Actions)
				{
					ActionComponent->AddAction(Action);
				}
			}
		}
	}
}
