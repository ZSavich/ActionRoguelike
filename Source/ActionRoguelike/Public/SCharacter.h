// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class ASMagicProjectile;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** General SCharacter's Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** General SCharacter's Properties */
	UPROPERTY(EditAnywhere, Category = "Sockets", Meta = (AllowPrivateAccess = "true"))
	FName HandSocketName;

	/** Enhanced Input System */
	UPROPERTY(EditAnywhere, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMapping;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* LookMouseAction;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;

	/** Projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASMagicProjectile> MagicProjectileClass;

public:
	ASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Enhanced Input Callbacks */
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_PrimaryAttack(const FInputActionValue& InputActionValue);
	
};
