// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "BrainComponent.h"
#include "SAIController.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/Image.h"
#include "UMG/SWorldUserWidget.h"

ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
    
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    if(!ensure(PawnSensingComp) || !ensure(AttributeComp)) return;
    
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
    AttributeComp->OnDead.AddDynamic(this, &ASAICharacter::OnDeadHandle);
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
    
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwnerAttributeComp, float CurrentHealth, float Delta)
{
    if(Delta < 0)
    {
        if(InstigatorActor == this) return;
        SetTargetActor(InstigatorActor);

        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
    }

    if(!ActiveHealthBar)
    {
        ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
        if(ActiveHealthBar)
        {
            ActiveHealthBar->AttachedActor = this;
            ActiveHealthBar->AddToViewport();
        }
    }
    if(ActiveHealthBar)
    {
        const auto HealthPercent = AttributeComp->GetHealthByPercent();
        const auto DynMaterial = ActiveHealthBar->HealthBarImage->GetDynamicMaterial();
        if(DynMaterial)
            DynMaterial->SetScalarParameterValue("HealthPercent", HealthPercent);
    }
}

void ASAICharacter::OnDeadHandle(AActor* InstigatorActor, AActor* VictimActor)
{    
    const auto AIController = GetController<ASAIController>();
    if(AIController)
        AIController->GetBrainComponent()->StopLogic("Killed");

    if(ActiveHealthBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("ActiveHealthBar"));
        ActiveHealthBar->RemoveFromParent();
    }
    
    GetMesh()->SetAllBodiesSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName("Ragdoll");

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    SetLifeSpan(10.f);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
    const auto Player = Cast<ASCharacter>(NewTarget);
    if(!Player) return;
    
    const auto AIController = GetController<ASAIController>();
    if(!AIController) return;

    AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
}

void ASAICharacter::OnPawnSeen(APawn* TargetPawn)
{
    SetTargetActor(TargetPawn);
}
