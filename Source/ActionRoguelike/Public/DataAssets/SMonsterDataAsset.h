// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "Engine/DataAsset.h"
#include "SMonsterDataAsset.generated.h"

class ASAICharacter;

UCLASS()
class ACTIONROGUELIKE_API USMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Class of a bot that will be spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASAICharacter> BotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<USAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
