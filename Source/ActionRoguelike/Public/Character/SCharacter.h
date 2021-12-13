// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ASBaseProjectile;
class USInteractionComponent;
class USAttributeComponent;

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

    UPROPERTY(EditDefaultsOnly, Category="Projectiles")
    TSubclassOf<ASBaseProjectile> MagicProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category="Projectiles")
    TSubclassOf<ASBaseProjectile> BlackholeProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category="Projectiles")
    TSubclassOf<ASBaseProjectile> TeleportProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category="Animations")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category="Debug")
    bool bDrawDebugInformation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects")
    FName TimeToHitParamName;

    FTimerHandle TimerHandle_PrimaryAttack;
    
public:
    ASCharacter();
    
    UPROPERTY(EditDefaultsOnly, Category="Sockets")
    FName MuzzleSocketName;

protected:
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* AttributeComponent, float CurrentHealth, float Delta);

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);

    void SpawnProjectile(const TSubclassOf<ASBaseProjectile> ProjectileClass);
    
    void PrimaryAttack();
    void PrimaryAttack_TimeElapsed();

    void BlackholeProjectile();
    void BlackholeProjectile_TimeElapsed();

    void TeleportProjectile();
    void TeleportProjectile_TimeElapsed();
    
    void PrimaryInteract();
    
    /** Debug Functions **/
    void DrawDebugOrientVectors() const;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;
};
