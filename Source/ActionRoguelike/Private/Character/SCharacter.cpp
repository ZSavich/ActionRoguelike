// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(GetRootComponent());

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(const float Amount)
{
    if(!Amount) return;
    
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASCharacter::MoveRight(const float Amount)
{
    if(!Amount) return;

    AddMovementInput(GetActorRightVector(), Amount);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
}

