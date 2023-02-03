// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Components/SAttributeComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Projectiles/SMagicProjectile.h"
#include "Components/SInteractionComponent.h"

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

	// Create an interact component
	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractComponent"));

	// Create an attribute component
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

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

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
		AttributeComponent->ApplyHealthChange(this, 0);
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

		// PrimaryInteract
		if (PrimaryInteractAction)
		{
			EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Completed, this, &ASCharacter::Input_PrimaryInteract);
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
	if (TimerHandle_SpawnProjectile.IsValid()) return;
	
	PlayAnimMontage(PrimaryAttackMontage);

	const FTimerDelegate SpawnProjectileDelegate = FTimerDelegate::CreateUObject(this, &ASCharacter::SpawnProjectile_TimeElapsed, PrimaryAttackClass);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnProjectile, SpawnProjectileDelegate, 0.2f , false);
}

void ASCharacter::Input_PrimaryInteract(const FInputActionValue& InputActionValue)
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASCharacter::Input_PrimaryAbility(const FInputActionValue& InputActionValue)
{
	if (TimerHandle_SpawnProjectile.IsValid()) return;
	
	PlayAnimMontage(PrimaryAttackMontage);
	
	const FTimerDelegate SpawnProjectileDelegate = FTimerDelegate::CreateUObject(this, &ASCharacter::SpawnProjectile_TimeElapsed, PrimaryAbilityClass);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnProjectile, SpawnProjectileDelegate, 0.2f , false);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
                                  float Delta)
{
	// If the Player was killed, we need to disable inputs
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(GetController<APlayerController>());
		return;
	}

	// Show the shining effect whenever the bot gets damage
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	}
}

void ASCharacter::SpawnProjectile_TimeElapsed(TSubclassOf<ASProjectileBase> ProjectileClass)
{
	if (TimerHandle_SpawnProjectile.IsValid())
	{
		TimerHandle_SpawnProjectile.Invalidate();
	}
	
	if (!GetMesh() || !IsValid(PrimaryAttackClass) || !GetWorld() || !FollowCamera) return; 

	// Find Rotation of a target
	FHitResult HitResult;
	
	const FVector StartTrace = FollowCamera->GetComponentLocation();
	const FVector EndTrace = StartTrace + (FollowCamera->GetComponentRotation().Vector() * 50000);
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		ECC_Visibility);

	const FVector TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;

	// Spawn Projectile
	const FVector SpawnLocation = GetMesh()->GetSocketLocation(HandSocketName); // Get Location of the character's muzzle socket
	const FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetPoint - SpawnLocation).Rotator(); // Get Rotation to the Target 
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<ASProjectileBase>(ProjectileClass, SpawnLocation, TargetRotation, SpawnParams);
}

