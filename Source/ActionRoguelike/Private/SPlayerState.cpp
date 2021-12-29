// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "Net/UnrealNetwork.h"

void ASPlayerState::AddCredits(const float Amount)
{
    Credits = Credits + Amount;
    OnCreditsChanged.Broadcast(Credits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASPlayerState, Credits);
}
