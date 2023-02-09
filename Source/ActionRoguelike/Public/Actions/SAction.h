// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SActionComponent.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	FName ActionName;

	/** Tags added to owning actor when activated, removed when action stopped */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/** Action can only start if Owning Actor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
	bool bIsActive;

public:
	USAction();
	USAction(const FName Name) : ActionName(Name) {};
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool StopAction(AActor* Instigator);

	bool CanStart() const;
	bool IsRunning() const { return bIsActive; }
	
	FORCEINLINE const FName& GetActionName() const { return ActionName; }
	
	FORCEINLINE bool operator()(const FName& Name) const { return ActionName == Name; }

protected:
	UFUNCTION(BlueprintCallable)
	USActionComponent* GetOwningComponent() const;
};
