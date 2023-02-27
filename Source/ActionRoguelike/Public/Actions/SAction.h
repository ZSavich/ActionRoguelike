// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SActionComponent.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	TObjectPtr<AActor> Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	bool bAutoStart;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float RageCost;

	/** Tags added to owning actor when activated, removed when action stopped */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/** Action can only start if Owning Actor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_ActionRepData");
	FActionRepData ActionRepData;

	UPROPERTY(Replicated)
	TObjectPtr<USActionComponent> OwnActionComponent;

public:
	USAction();
	USAction(const FName Name) : ActionName(Name) {};

	void Initialize(USActionComponent* ActionComponent);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool StopAction(AActor* Instigator);

	bool CanStart() const;
	bool IsRunning() const { return ActionRepData.bIsRunning; }
	
	FORCEINLINE const FName& GetActionName() const { return ActionName; }
	FORCEINLINE bool ShouldAutoStart() const { return bAutoStart; }
	
	FORCEINLINE bool operator()(const FName& Name) const { return ActionName == Name; }

protected:
	UFUNCTION(BlueprintCallable)
	USActionComponent* GetOwningComponent() const;

	/** Multiplayer Functions */
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
	UFUNCTION()
	void OnRep_ActionRepData();
};
