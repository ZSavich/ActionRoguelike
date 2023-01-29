// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributeComponent;
class UWidgetComponent;
class USDamagePopupWidget;

UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

protected:
	/** Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* DamageWidgetComponent;
	
public:	
	ASTargetDummy();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
