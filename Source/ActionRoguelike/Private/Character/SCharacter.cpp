// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectile/SMagicProjectile.h"
#include "Components/SInteractionComponent.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

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

    MuzzleSocketName = "Muzzle_01";
    bDrawDebugInformation = false;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // DrawDebugOrientVectors();
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

void ASCharacter::PrimaryAttack()
{
    PlayAnimMontage(AttackMontage);
    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
    ensure(MagicProjectileClass);
   
    /** LineTrace  **/
    /** It helps to find the Rotation for Projectile **/
    
    FVector ViewPointLocation;
    FRotator ViewPointRotation;
    GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

    const auto StartTraceVector = ViewPointLocation;
    auto EndTraceVector = ViewPointLocation + (ViewPointRotation.Vector() * 2000.f);

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(this);
    
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByObjectType(HitResult, StartTraceVector, EndTraceVector, {}, CollisionQueryParams);

    if(HitResult.bBlockingHit)
    {
        EndTraceVector = HitResult.ImpactPoint;
    }
    
    if(bDrawDebugInformation)
    {
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 32, FColor::Green, false, 5.f, 0.f, 1.f);
        DrawDebugLine(GetWorld(), StartTraceVector, EndTraceVector, FColor::Green, false, 5.f,0.f,1.f);
    }
    
    /** End LineTrace **/
    
    const auto SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
    const auto SpawnRotation = FRotationMatrix::MakeFromX(EndTraceVector-SpawnLocation).Rotator();

    const auto SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    SpawnParams.Owner = this;
    
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    const auto Projectile = GetWorld()->SpawnActor<ASMagicProjectile>(MagicProjectileClass, SpawnTransform, SpawnParams);

    // Fixing the bug when The Projectile hit The Owner
    MoveIgnoreActorAdd(Projectile);
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
