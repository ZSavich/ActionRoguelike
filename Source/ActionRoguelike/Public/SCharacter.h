// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Projectiles/SProjectileBase.h"
#include "SCharacter.generated.h"

struct FInputActionValue;
class USInteractionComponent;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class USAttributeComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USAttributeComponent> AttributeComponent;

	/** General SCharacter's Properties */
	UPROPERTY(EditAnywhere, Category = "Sockets", Meta = (AllowPrivateAccess = "true"))
	FName HandSocketName;

	/** Enhanced Input System */
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* LookMouseAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryInteractAction;

	/** Projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASProjectileBase> PrimaryAttackClass;

	/** Animation Montages */
	UPROPERTY(EditAnywhere, Category = "Montages", Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PrimaryAttackMontage;

	/** Timers */
	FTimerHandle TimerHandle_PrimaryAttack;
	
public:
	ASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Enhanced Input Callbacks */
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_PrimaryAttack(const FInputActionValue& InputActionValue);
	void Input_PrimaryInteract(const FInputActionValue& InputActionValue);

private:
	/** Timer's callbacks */
	void PrimaryAttack_TimeElapsed();
	
};
