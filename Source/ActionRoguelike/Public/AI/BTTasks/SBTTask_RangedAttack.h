// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

class ASProjectileBase;

UCLASS()
class ACTIONROGUELIKE_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Parameters")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	FName MuzzleSocketName;
	
public:
	USBTTask_RangedAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};