// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
	/** Chest's Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> LidMesh;

	/** Chest's Properties*/
	UPROPERTY(EditAnywhere, Category = "Properties")
	float TargetPitch;
	
public:	
	ASItemChest();

	/** Override IGameplayInterface functions */
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

};
