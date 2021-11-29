// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ASMagicProjectile;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(VisibleAnywhere, Category="Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, Category="Attack")
    TSubclassOf<ASMagicProjectile> MagicProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category="Sockets")
    FName MuzzleSocketName;

    UPROPERTY(VisibleAnywhere, Category="Interaction")
    USInteractionComponent* InteractionComp;

    UPROPERTY(EditDefaultsOnly, Category="Attack")
    UAnimMontage* AttackMontage;

    FTimerHandle TimerHandle_PrimaryAttack;
    
public:
	ASCharacter();

protected:
	virtual void BeginPlay() override;

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);

    void PrimaryAttack();

    void PrimaryAttack_TimeElapsed();
    
    void PrimaryInteract();
    
    /** Debug Functions **/
    void DrawDebugOrientVectors() const;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
