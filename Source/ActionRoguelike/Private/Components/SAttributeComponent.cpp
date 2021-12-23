// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SAttributeComponent.h"
#include "SGameMode.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.f, TEXT("Multiplies damage by the specified amount."), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;

    bRageActivate = false;
    MaxRage = 100.f;
    CurrentRage = 25.f;
    RageMultiply = 1.0f;

    SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::IsAlive() const
{
    return CurrentHealth > 0.f; 
}

void USAttributeComponent::Kill(AActor* InstigatorActor)
{
    if(IsAlive())
        ApplyHealthChange(InstigatorActor, -MaxHealth);
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
    if(!IsAlive() || !GetOwner()->CanBeDamaged()) return false;
    
    if(CurrentHealth == MaxHealth && Delta >= 0.f)
    {
        OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, 0.f);
        return false;
    }

    const auto OldHealth = CurrentHealth;
    const auto MultiDelta = CVarDamageMultiplier.GetValueOnGameThread() * Delta;
    CurrentHealth = FMath::Clamp(CurrentHealth + MultiDelta, 0.f, MaxHealth);
    const auto ActualDelta = CurrentHealth - OldHealth;

    if(bRageActivate)
        ApplyRageChange(-ActualDelta);
    
    MulticastHealthChanged(InstigatorActor, CurrentHealth, ActualDelta);

    if(!IsAlive())
    {
        OnDead.Broadcast(InstigatorActor, GetOwner());
        const auto GM = GetWorld()->GetAuthGameMode<ASGameMode>();
        if(GM)
            GM->OnActorKilled(GetOwner(), InstigatorActor);
    }
    return true;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* Actor)
{
    if(!Actor) return nullptr;

    const auto AttributeComp = Actor->FindComponentByClass<USAttributeComponent>();
    if(!AttributeComp) return nullptr;

    return AttributeComp;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
    if(!Actor) return false;

    const auto AttributeComp = GetAttributes(Actor);
    if(!AttributeComp || !AttributeComp->IsAlive()) return false;

    return true;
}

bool USAttributeComponent::ApplyRageChange(const float Delta)
{
    if(!IsAlive() || CurrentRage == MaxRage && Delta > 0.f) return false;

    const auto RageDelta = Delta * RageMultiply;

    if(CurrentRage + RageDelta < 0.f) return false;
    CurrentRage = FMath::Clamp(CurrentRage + RageDelta, 0.f, MaxRage);
    
    OnRageChanged.Broadcast(this, CurrentRage, RageDelta);
    return true;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float Health, float Delta)
{
    OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, Delta);   
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USAttributeComponent, CurrentHealth);
    DOREPLIFETIME(USAttributeComponent, MaxHealth);
}