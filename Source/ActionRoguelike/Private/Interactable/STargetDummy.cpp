// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/STargetDummy.h"
#include "Components/SAttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/SDamagePopupWidget.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Create a static mesh component
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
	
	// Create an attribute component
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	// Create a damage widget component
	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	DamageWidgetComponent->SetupAttachment(GetRootComponent());
	DamageWidgetComponent->Deactivate();
}

void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
	}
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (BaseMesh)
	{
		BaseMesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	}
	if (DamageWidgetComponent)
	{
		if (USDamagePopupWidget* DamageWidget = Cast<USDamagePopupWidget>(DamageWidgetComponent->GetUserWidgetObject()))
		{
			DamageWidgetComponent->Activate();
			DamageWidget->SetDamageText(Delta);
		}
	}
}