// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SBaseProjectile.h"
#include "Components/SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"

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

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

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
    PlayerInputComponent->BindAction("BlackholeProjectile", IE_Pressed, this, &ASCharacter::BlackholeProjectile);
    PlayerInputComponent->BindAction("TeleportProjectile", IE_Pressed, this, &ASCharacter::TeleportProjectile);
}

void ASCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
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
    if(GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack)) return;
    
    PlayAnimMontage(AttackMontage);
    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackholeProjectile()
{
    if(GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack)) return;
    
    PlayAnimMontage(AttackMontage);
    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackholeProjectile_TimeElapsed, 0.2f);
}

void ASCharacter::TeleportProjectile()
{
    if(GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack)) return;

    PlayAnimMontage(AttackMontage);
    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::TeleportProjectile_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
    if(!ensure(MagicProjectileClass)) return;
    SpawnProjectile(MagicProjectileClass);
}

void ASCharacter::BlackholeProjectile_TimeElapsed()
{
    if(!ensure(BlackholeProjectileClass)) return;
    SpawnProjectile(BlackholeProjectileClass);
}

void ASCharacter::TeleportProjectile_TimeElapsed()
{
    if(!ensure(TeleportProjectileClass)) return;
    SpawnProjectile(TeleportProjectileClass);
}

void ASCharacter::SpawnProjectile(const TSubclassOf<ASBaseProjectile> ProjectileClass)
{
    /** LineTrace **/
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
    
    const auto Projectile = GetWorld()->SpawnActor<ASBaseProjectile>(ProjectileClass, SpawnTransform, SpawnParams);

    // Fixing the bug when The Projectile hit The Owner
    MoveIgnoreActorAdd(Projectile);
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
            MI->SetScalarParameterValue("TimeToHit", GetWorld()->TimeSeconds);
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
