// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/SGameplayInterface.h"

// Add default functionality here for any ISGameplayInterface functions that are not pure virtual.
FText ISGameplayInterface::GetInteractText(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}
