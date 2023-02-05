// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Static Functions */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", Meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

protected:
	/** Attributes */
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

public:
	USAttributeComponent();

	bool ApplyHealthChange(AActor* Instigator, const float Delta);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercentage() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return Health > 0.f; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsFullHealth() const { return Health >= MaxHealth; }

	/** Helper Functions */
	UFUNCTION()
	bool KillSelf();
};
