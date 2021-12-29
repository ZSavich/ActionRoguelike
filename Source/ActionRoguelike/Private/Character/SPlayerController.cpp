// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPlayerController.h"

void ASPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);
    OnPawnChanged.Broadcast(InPawn);
}
