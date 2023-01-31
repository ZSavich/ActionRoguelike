// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

ASAICharacter::ASAICharacter()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}