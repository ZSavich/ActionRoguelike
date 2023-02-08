// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	FName ActionName;

	bool bIsActive;

public:
	USAction();
	USAction(const FName Name) : ActionName(Name) {};
	
	UFUNCTION(BlueprintNativeEvent)
	bool StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	bool StopAction(AActor* Instigator);

	FORCEINLINE const FName& GetActionName() const { return ActionName; }

	FORCEINLINE bool operator()(const FName& Name) const { return ActionName == Name; }
};
