// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SInteractionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ASMagicProjectile;
class ISGameplayInterface;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(VisibleAnywhere, Category="Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    TSubclassOf<ASMagicProjectile> MagicProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    FName MuzzleSocketName;

    UPROPERTY(VisibleAnywhere, Category="Components")
    USInteractionComponent* InteractionComp;
    
public:
	ASCharacter();

protected:
	virtual void BeginPlay() override;

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);

    void PrimaryAttack();

    void PrimaryInteract();
    
    /** Debug Functions **/
    void DrawDebugOrientVectors() const;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
