// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category="Mesh")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
    UStaticMeshComponent* LidMesh;

    UPROPERTY(EditAnywhere, Category="Interact")
    float TargetRotation;

    UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly) // RepNotify
    bool bLidOpened;
	
public:	
	ASItemChest();

protected:
    UFUNCTION()
    void OnRep_LidOpened();

public:	
    virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
