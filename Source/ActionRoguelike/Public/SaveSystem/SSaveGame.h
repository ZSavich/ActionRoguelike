// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SSaveGame.generated.h"

/* Structure for Actors that have SaveableActorInterface */
USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	/* Identifier for which Actor this belongs to */
	UPROPERTY()
	FName ActorName;

	/* For movable Actors, keep location, rotation and scale */
	UPROPERTY()
	FTransform ActorTransform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
	TArray<uint8> ActorData;
};

/* Structure for Player that need to save */
USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

	/* Player's data */
	UPROPERTY()
	float Credits;
	
	UPROPERTY()
	FString PlayerId;

	/* Player's location and rotation */
	UPROPERTY()
	FTransform PlayerTransform;
	
	UPROPERTY()
	bool bResumeAtTransform;
};

struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FSaveGameArchive(FArchive& InInnerArchive) : FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		// Find only variables with UPROPERTY(SaveGame)
		ArIsSaveGame = true;
	}
};

UCLASS()
class ACTIONROGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/* Game's data */
	UPROPERTY()
	FName GameID;
	
	UPROPERTY()
	FDateTime Timestamp;
	
	/* Actors stored from level */
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	/* Players that stored */
	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;
	
};
