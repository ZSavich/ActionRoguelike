// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

void ASPlayerState::AddCredits(const float Amount)
{
    Credits = Credits + Amount;
    OnCreditsChanged.Broadcast(Credits);
}
