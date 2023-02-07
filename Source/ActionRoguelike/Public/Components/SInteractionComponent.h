// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Interaction Component's Properties */
	UPROPERTY(EditDefaultsOnly, Category = "Properties", Meta = (AllowPrivateAccess = "true"))
	float TraceSphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", Meta = (AllowPrivateAccess = "true"))
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", Meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceObjectType;
	
public:	
	USInteractionComponent();

	void PrimaryInteract();
};
