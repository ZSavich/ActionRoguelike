// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

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