// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SCoinPickup.h"
#include "SPlayerState.h"

ASCoinPickup::ASCoinPickup()
{
	GrantCredits = 100.f;
}

bool ASCoinPickup::ActivatePickup(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			return PlayerState->ApplyCreditChange(GrantCredits);
		}
	}
	
	return false;
}
