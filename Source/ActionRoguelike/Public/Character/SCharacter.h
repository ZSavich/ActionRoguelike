// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USInteractionComponent* InteractionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USAttributeComponent* AttributeComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USActionComponent* ActionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects")
    FName TimeToHitParamName;
    
public:
    ASCharacter();

protected:

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* AttributeComponent, float CurrentHealth, float Delta);

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);

    void SprintStart();
    void SprintStop();

    void PrimaryAttack();
    void BlackholeProjectile();
    void TeleportProjectile();
    
    void PrimaryInteract();
    
    /** Debug Functions **/
    void DrawDebugOrientVectors() const;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;
    
    UFUNCTION(Exec)
    void HealSelf(float Amount = 100);

    virtual FVector GetPawnViewLocation() const override;
};
