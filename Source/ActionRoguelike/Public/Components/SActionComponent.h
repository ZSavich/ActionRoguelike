// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, class USActionComponent*, OwningComp, USAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	FGameplayTagContainer  ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnActionStateChanged OnActionStopped;
	
protected:
	/** Action Component Properties */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Properties")
	TArray<USAction*> ActiveActions;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
public:	
	USActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	/* Replicates USAction objects because it's UObject */
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

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

protected:
	/** Multiplayer Functions */
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, const FName& ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, const FName& ActionName);
};
