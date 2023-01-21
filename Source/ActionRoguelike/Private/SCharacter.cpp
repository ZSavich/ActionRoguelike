// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Projectiles/SMagicProjectile.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Don't rotate when the controller rotates. Let that just affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll= false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // At this rotation rate
	
	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(GetRootComponent());

	// Create a follow camera 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetupAttachment(CameraBoom);

	// General SCharacter's Properties
	HandSocketName = FName("Muzzle_01");
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Clear and Add Input Mapping Context
	if (DefaultMapping)
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(DefaultMapping, 0);
			}
		}
	}
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Input_Move);
		}

		// Looking
		if (LookMouseAction)
		{
			EnhancedInputComponent->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ASCharacter::Input_LookMouse);
		}

		// PrimaryAttack
		if (PrimaryAttackAction)
		{
			EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Completed, this, &ASCharacter::Input_PrimaryAttack);
		}

		// Jump
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
	}
}

void ASCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		if (Value.X != 0.f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ASCharacter::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

void ASCharacter::Input_PrimaryAttack(const FInputActionValue& InputActionValue)
{
	if (!GetMesh() || !IsValid(MagicProjectileClass) || !GetWorld()) return; 
	
	const FVector SpawnLocation = GetMesh()->GetSocketLocation(HandSocketName); // Get location of the character's muzzle socket 
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ASMagicProjectile>(MagicProjectileClass, SpawnLocation, GetControlRotation(), SpawnParams);
}

