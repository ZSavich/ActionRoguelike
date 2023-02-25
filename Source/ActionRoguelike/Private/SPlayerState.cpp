// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SaveSystem/SSaveGame.h"

ASPlayerState::ASPlayerState()
{
	Credits = 0.f;
}

bool ASPlayerState::ApplyCreditChange(const float Amount)
{
	// If we don't have enough credits
	if (Credits + Amount < 0.f)
	{
		return false;
	}

	Credits += Amount;
	OnCreditsChange.Broadcast(Credits);
	return true;
}

void ASPlayerState::SetCredits(const float Amount)
{
	ApplyCreditChange(Amount);
}

void ASPlayerState::SavePlayerState(USSaveGame* SaveObject) const
{
	if (SaveObject)
	{
		FPlayerSaveData SaveData;
		// Gather relevant data for player
		SaveData.Credits = Credits;
		// Stored as FString for simplicity (original Steam ID is uint64)
		SaveData.PlayerId = GetUniqueId().ToString();

		// May not alive while save
		if (const APawn* Pawn = GetPawn())
		{
			SaveData.PlayerTransform = Pawn->GetTransform();
			SaveData.bResumeAtTransform = true;
		}

		SaveObject->SavedPlayers.Add(SaveData);
		UE_LOG(LogTemp, Log, TEXT("EUD::Saved Player State."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Can't save Player State. SaveObject is not valid."));
		return;
	}
}

void ASPlayerState::LoadPlayerState(const USSaveGame* SaveObject)
{
 	if (SaveObject && SaveObject->SavedPlayers.IsValidIndex(0))
	{
		const FPlayerSaveData* FoundData = &SaveObject->SavedPlayers[0];
		if (FoundData)
		{
			ApplyCreditChange(FoundData->Credits);
			UE_LOG(LogTemp, Log, TEXT("EUD::Loaded Player State."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EUD::Can't find save data for this player state."));	
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Can't load Player State. SaveObject is not valid."));
		return;
	}
}

void ASPlayerState::OverrideSpawnTransform(const USSaveGame* SaveObject)
{
	if (!SaveObject || !SaveObject->SavedPlayers.IsValidIndex(0)) return;
	
	const FPlayerSaveData FoundData = SaveObject->SavedPlayers[0];
	if (FoundData.bResumeAtTransform)
	{
		if (APawn* Pawn = GetPawn())
		{
			Pawn->SetActorTransform(FoundData.PlayerTransform);
			UE_LOG(LogTemp, Warning, TEXT("EUD::Overrided spawn transform for the player."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EUD::Can't override spawn transform for player."));
	}
}

// Multiplayer Functions
void ASPlayerState::OnRep_Credits()
{
	OnCreditsChange.Broadcast(Credits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
	//DOREPLIFETIME_CONDITION(ASPlayerState, Credits, COND_OwnerOnly);
}