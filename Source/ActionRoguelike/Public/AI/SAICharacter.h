// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Bot's Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	/** Bot's Properties */
	UPROPERTY(EditAnywhere, Category = "Properties")
	float LifeSpanTime;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName TimeToHitParamName;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(Transient)
	USWorldUserWidget* HealthBarWidget;
	
public:
	ASAICharacter();
	
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void HandleOnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTargetActor);
	AActor* GetTargetActor();
};
