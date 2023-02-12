// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRageChanged OnRageChanged;

protected:
	/** Attributes */
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Rage;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxRage;

public:
	USAttributeComponent();

	/** Static Functions */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", Meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	/** Health Attribute Handlers */
	bool ApplyHealthChange(AActor* Instigator, float Delta);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercentage() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return Health > 0.f; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsFullHealth() const { return Health >= MaxHealth; }

	/** Rage Attribute Handlers */
	bool ApplyRageChange(float Delta);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRagePercentage() const { return Rage / MaxRage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRage() const { return Rage; }
	
	/** Helper Functions */
	UFUNCTION()
	bool KillSelf();
};
