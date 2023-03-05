// Fill out your copyright notice in the Description page of Project Settings.

#include "SAnimInstance.h"
#include "Components/SActionComponent.h"

USAnimInstance::USAnimInstance()
{
	bIsStunned = false;
	bIsSprinting = false;
}

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get the character's Action Component 
	if (AActor* OwningActor = GetOwningActor())
	{
		ActionComponent = USActionComponent::GetActionComponent(OwningActor);

		// Find Gameplay Tags that we will check
		StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
		SprintingTag = FGameplayTag::RequestGameplayTag("Action.Sprinting");
	}
}

void USAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (ActionComponent)
	{
		bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
		bIsSprinting = ActionComponent->ActiveGameplayTags.HasTag(SprintingTag);
	}
}
