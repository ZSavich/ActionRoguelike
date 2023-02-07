// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

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
