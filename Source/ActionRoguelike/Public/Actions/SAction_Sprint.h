// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_Sprint.generated.h"

class UCharacterMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API USAction_Sprint : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float SprintMultiply;

	UPROPERTY(Transient)
	UCharacterMovementComponent* MovementComponent;
	
public:
	USAction_Sprint();

	virtual bool StartAction_Implementation(AActor* Instigator) override;
	virtual bool StopAction_Implementation(AActor* Instigator) override;
};
