// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SCharacter.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"

ASCharacter::ASCharacter()
{
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->TargetArmLength = 200.f;
    SpringArmComp->SocketOffset = FVector(0.f, 50.f, 60.f);
    SpringArmComp->SetupAttachment(GetRootComponent());

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);

    InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

    ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

    TimeToHitParamName = "TimeToHit";
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
    PlayerInputComponent->BindAction("BlackholeProjectile", IE_Pressed, this, &ASCharacter::BlackholeProjectile);
    PlayerInputComponent->BindAction("TeleportProjectile", IE_Pressed, this, &ASCharacter::TeleportProjectile);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
    if(AttributeComp->IsAlive())
        AttributeComp->ApplyHealthChange(this, Amount);
}

FVector ASCharacter::GetPawnViewLocation() const
{
    return CameraComp->GetComponentLocation();
}

void ASCharacter::PrimaryInteract()
{
    if(!InteractionComp) return;
    
    InteractionComp->PrimaryInteract();
}

void ASCharacter::MoveForward(const float Amount)
{
    if(!Amount) return;

    // Rotate the Pawn to the camera forward view
    auto ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.f;
    ControlRot.Roll = 0.f;
    
    AddMovementInput(ControlRot.Vector(), Amount);
}

void ASCharacter::MoveRight(const float Amount)
{
    if(!Amount) return;

    // Rotate the Pawn to the right/left 
    auto ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.f;
    ControlRot.Roll = 0.f;

    const auto RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
    
    AddMovementInput(RightVector, Amount);
}

void ASCharacter::SprintStart()
{
    ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
    ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
    ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackholeProjectile()
{
    ActionComp->StartActionByName(this, "BlackholeProjectile");
}

void ASCharacter::TeleportProjectile()
{
    ActionComp->StartActionByName(this, "TeleportProjectile");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* AttributeComponent, float CurrentHealth, float Delta)
{
    if(CurrentHealth <= 0.f)
    {
        const auto PC = GetController<APlayerController>();
        DisableInput(PC);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SetLifeSpan(10.f);
        return;
    }
    /** Set Flash Material when get damage **/
    if(Delta < 0.f)
    {
        const auto MaterialsNum = GetMesh()->GetMaterials().Num(); 
        for(int i = 0; i < MaterialsNum; i++)
        {
            const auto MI = GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(i,GetMesh()->GetMaterial(i));
            MI->SetScalarParameterValue(TimeToHitParamName, GetWorld()->TimeSeconds);
        }
    }
    /** **/
}

void ASCharacter::DrawDebugOrientVectors() const
{
    // -- Rotation Visualization -- //
    constexpr  float DrawScale = 100.0f;
    constexpr float Thickness = 5.0f;

    FVector LineStart = GetActorLocation();
    // Offset to the right of pawn
    LineStart += GetActorRightVector() * 100.0f;
    // Set line end in direction of the actor's forward
    const FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
    // Draw Actor's Direction
    DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

    const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
    // Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
    DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}
