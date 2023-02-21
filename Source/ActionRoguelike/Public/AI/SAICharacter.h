// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class USWorldUserWidget;
class UPawnSensingComponent;
class USActionComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USActionComponent> ActionComponent;

	/** Bot's Widgets */
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<USWorldUserWidget> TargetSpottedWidgetClass;
	
	UPROPERTY(Transient)
	USWorldUserWidget* HealthBarWidget;
	

	/** Bot's Properties */
	UPROPERTY(EditAnywhere, Category = "Properties")
	float LifeSpanTime;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName TimeToHitParamName;
	
	UPROPERTY(EditAnywhere, Category = "Properties|Drop")
	float RewardCredits;
	
public:
	ASAICharacter();
	
	virtual void BeginPlay() override;

	FORCEINLINE float GetRewardCredits() const { return RewardCredits; }

protected:
	UFUNCTION()
	void HandleOnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTargetActor);
	AActor* GetTargetActor();

	/** Multiplayer Functions */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnSeePawn();
};
