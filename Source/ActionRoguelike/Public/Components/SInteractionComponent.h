// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UMG/SWorldUserWidget.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:    
    UPROPERTY(EditDefaultsOnly, Category="Trace")
    float TraceRadius;

    UPROPERTY(EditDefaultsOnly, Category="Trace")
    float TraceDistance;

    UPROPERTY(EditDefaultsOnly, Category="Trace")
    TEnumAsByte<ECollisionChannel> CollisionChannel;

    UPROPERTY(EditDefaultsOnly, Category="UMG")
    TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

    UPROPERTY()
    USWorldUserWidget* DefaultWidgetInstance;

    UPROPERTY()
    AActor* FocusedActor;
    

public:	
	USInteractionComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void PrimaryInteract();

protected:
    void FindBestInteractable();
    
    UFUNCTION(Server, Reliable)
    void ServerInteract(AActor* ActorInteract);
};
