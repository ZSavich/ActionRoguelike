// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/SMonsterDataAsset.h"

FPrimaryAssetId USMonsterDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Bots", GetFName());
}
