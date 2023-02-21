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
class USActionComponent;
class ASPlayerState;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USActionComponent> ActionComponent;

	/** Enhanced Input System */
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

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

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;

	/** Projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASProjectileBase> PrimaryAbilityClass;

public:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Actions")
	FName GrantActionName;
	
public:
	ASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStateReady();
	
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
	void Input_Sprint(const FInputActionValue& InputActionValue);
	void Input_Parry(const FInputActionValue& InputActionValue);

	/** Attribute Component Callbacks */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);
};
