// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	USAnimInstance();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<USActionComponent> ActionComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsSprinting;

private:
	FGameplayTag StunnedTag;
	FGameplayTag SprintingTag;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

};
