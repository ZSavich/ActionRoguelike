// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	FGameplayTagContainer  ActiveGameplayTags;
	
protected:
	/** Action Component Properties */
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TArray<USAction*> ActiveActions;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
public:	
	USActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	/** Static Functions */
	UFUNCTION(BlueprintCallable)
	static USActionComponent* GetActionComponent(AActor* FromActor);
	
	/** Action Handlers */
	USAction* AddAction(TSubclassOf<USAction> ActionClass);
	void RemoveAction(USAction* Action);

	UFUNCTION(BlueprintCallable)
	bool StartActionByName(AActor* Instigator, const FName& ActionName);

	UFUNCTION(BlueprintCallable)
	bool StopActionByName(AActor* Instigator, const FName& ActionName);
		
};
