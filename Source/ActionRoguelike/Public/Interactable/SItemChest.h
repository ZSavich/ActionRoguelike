// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SGameplayInterface.h"
#include "SaveSystem/SSaveableActorInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface, public ISSaveableActorInterface
{
	GENERATED_BODY()

protected:
	/** Chest's Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> LidMesh;

	/** Chest's Properties*/
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadWrite, SaveGame)
	bool bLidOpened;
	
public:	
	ASItemChest();
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRep_LidOpened();

	virtual void ActorSaveDataLoaded_Implementation() override;

	virtual FText GetInteractText(APawn* InstigatorPawn) override;

};
