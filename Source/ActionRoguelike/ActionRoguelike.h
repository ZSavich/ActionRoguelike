// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** When you modify this, please note that the information can be saved with instances
 * also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define TRACECHANNEL_PROJECTILE ECC_GameTraceChannel1

// Quick tip print
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, text)

// General Log
DECLARE_LOG_CATEGORY_EXTERN(LogRogue, Log, All);
// Logging for AI
DECLARE_LOG_CATEGORY_EXTERN(LogRogueAI, Log, All);
