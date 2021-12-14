// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects")
    FName TimeToHitParamName;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UPawnSensingComponent* PawnSensingComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USAttributeComponent* AttributeComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets")
    TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Credits")
    float DeathCredits;
    
    UPROPERTY()
    USWorldUserWidget* ActiveHealthBar;
    
public:
    ASAICharacter();

protected:
    UFUNCTION()
    void OnDeadHandle(AActor* InstigatorActor, AActor* VictimActor);

    void SetTargetActor(AActor* NewTarget);
    
public:
    virtual void PostInitializeComponents() override;
    
    UFUNCTION()
    void OnPawnSeen(APawn* TargetPawn);
    
    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwnerAttributeComp, float CurrentHealth, float Delta);

    FORCEINLINE float GetDeathCredits() const { return DeathCredits; }
};