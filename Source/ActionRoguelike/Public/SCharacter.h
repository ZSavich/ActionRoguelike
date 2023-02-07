// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

struct FInputActionValue;
class USInteractionComponent;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class USAttributeComponent;
class ASProjectileBase;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** General SCharacter's Properties */
	UPROPERTY(EditAnywhere, Category = "Sockets", Meta = (AllowPrivateAccess = "true"))
	FName HandSocketName;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAbilityAction;

	/** Projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASProjectileBase> PrimaryAttackClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASProjectileBase> PrimaryAbilityClass;

	/** Animation Montages */
	UPROPERTY(EditAnywhere, Category = "Montages", Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PrimaryAttackMontage;

	/** Timers */
	FTimerHandle TimerHandle_SpawnProjectile;
	
public:
	ASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Console Command Functions */
	UFUNCTION(Exec)
	void HealSelf(const float Amount) const;

protected:
	/** Enhanced Input Callbacks */
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_PrimaryAttack(const FInputActionValue& InputActionValue);
	void Input_PrimaryInteract(const FInputActionValue& InputActionValue);
	void Input_PrimaryAbility(const FInputActionValue& InputActionValue);

	/** Attribute Component Callbacks */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

private:
	/** Timer's callbacks */
	void SpawnProjectile_TimeElapsed(TSubclassOf<ASProjectileBase> ProjectileClass);
	
};
