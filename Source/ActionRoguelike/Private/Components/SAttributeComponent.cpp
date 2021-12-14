// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SAttributeComponent.h"
#include "SGameMode.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.f, TEXT("Multiplies damage by the specified amount."), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
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
    
    OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, ActualDelta);

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