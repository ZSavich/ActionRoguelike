// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, AttributeComp, float, CurrentHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributeComponent*, AttributeComp, float, CurrentRage, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDead, AActor*, InstigatorActor, AActor*, VictimActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float CurrentHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float MaxHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
    bool bRageActivate;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage", meta = (EditCondition = "bRageActivate"))
    float CurrentRage;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage", meta = (EditCondition = "bRageActivate"))
    float MaxRage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage", meta = (EditCondition = "bRageActivate"))
    float RageMultiply;
    
public:
    UFUNCTION(BlueprintCallable, Category="Attributes")
    static USAttributeComponent* GetAttributes(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category="Attributes", meta = (DisplayName = "IsAlive"))
    static bool IsActorAlive(AActor* Actor);
    
	USAttributeComponent();
    
    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable)
    FOnRageChanged OnRageChanged;

    UPROPERTY(BlueprintAssignable)
    FOnDead OnDead;

    UFUNCTION(BlueprintCallable)
    bool ApplyHealthChange(AActor* InstigatorActor, const float Delta);

    UFUNCTION(BlueprintCallable)
    bool ApplyRageChange(const float Delta);
    
    UFUNCTION(BlueprintCallable)
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable)
    void Kill(AActor* InstigatorActor);
    
    FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
    FORCEINLINE float GetHealthByPercent() const { return CurrentHealth / MaxHealth; }
};
