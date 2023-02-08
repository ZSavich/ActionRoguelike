// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/SAction.h"

USAction::USAction()
{
	bIsActive = false;
}

bool USAction::StartAction_Implementation(AActor* Instigator)
{
	if (!bIsActive && Instigator)
	{
		UE_LOG(LogTemp, Log, TEXT("EUD::Started action %s"), *GetNameSafe(this));
		bIsActive = true;
		return true;
	}
	return false;
}

bool USAction::StopAction_Implementation(AActor* Instigator)
{
	if (bIsActive && Instigator)
	{
		UE_LOG(LogTemp, Log, TEXT("EUD::Stopped action %s"), *GetNameSafe(this));
		
		bIsActive = false;
		return true;
	}
	return false;
}