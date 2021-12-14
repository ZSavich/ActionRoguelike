// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pickups/SCoin.h"

ASCoin::ASCoin()
{
    CreditsForInteract = 10.f;
}

bool ASCoin::Effect(AActor* InstigatorActor)
{
    return true;
}
