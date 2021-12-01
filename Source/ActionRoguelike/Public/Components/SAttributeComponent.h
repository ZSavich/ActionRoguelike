// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, AttributeComp, float, CurrentHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float CurrentHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float MaxHealth;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;
    
public:	
	USAttributeComponent();


public:
    UFUNCTION(BlueprintCallable)
    bool ApplyHealthChange(const float Delta);

    FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
};
