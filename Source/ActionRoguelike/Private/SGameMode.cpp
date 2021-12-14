// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Templates/SharedPointer.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Do need to spawn bots?"), ECVF_Cheat);

ASGameMode::ASGameMode()
{
    SpawnTimerInterval = 2.f;
}

void ASGameMode::StartPlay()
{
    Super::StartPlay();
    if(!ensure(MinionClass) || !ensure(SpawnBotQuery) || !ensure(DifficultyCurve)) return;

    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameMode::SpawnBotsTimerElapsed, SpawnTimerInterval, true, 0.f);
}

void ASGameMode::KillAll()
{
    for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        const auto AttributeComp = USAttributeComponent::GetAttributes(*It);
        if(AttributeComp && AttributeComp->IsAlive())
            AttributeComp->Kill(this); // @fix: Add Player's Pointer
    }
}

void ASGameMode::OnActorKilled(AActor* VictimActor, AActor* InstigatorActor)
{
    if(!VictimActor) return;
    
    const auto VictimPlayer = Cast<ASCharacter>(VictimActor);
    if(VictimPlayer)
    {
        FTimerHandle TimerHandle_RespawnPlayer;
        FTimerDelegate TimerDelegate_RespawnPlayer;
        TimerDelegate_RespawnPlayer.BindUObject(this, &ASGameMode::RespawnPlayer, VictimPlayer->GetController());

        const auto RespawnDelay = 5.f;
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnPlayer, TimerDelegate_RespawnPlayer, RespawnDelay, false);
        return;
    }
}

void ASGameMode::SpawnBotsTimerElapsed()
{
    if(!CVarSpawnBots.GetValueOnGameThread()) return;
    
    const float MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());

    float BotsAlive = 0;
    for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        const auto Bot = *It;
        const auto AttributeComp = USAttributeComponent::GetAttributes(Bot);
        if(AttributeComp && AttributeComp->IsAlive()) BotsAlive++;
    }

    if(BotsAlive < MaxBotsCount)
    {
        FEnvQueryRequest(SpawnBotQuery, this).Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ASGameMode::OnQueryCompleted);
    }
}

void ASGameMode::OnQueryCompleted(const TSharedPtr<FEnvQueryResult> QueryResult) const
{
    if(!QueryResult->IsSuccessful()) return;

    TArray<FVector> SpawnLocations;
    QueryResult->GetAllAsLocations(SpawnLocations);

    if(SpawnLocations.IsValidIndex(0))
    {
        GetWorld()->SpawnActor<ASAICharacter>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);   
    }
}

void ASGameMode::RespawnPlayer(AController* Controller)
{
    if(!Controller) return;

    Controller->UnPossess();
    RestartPlayer(Controller);
}
