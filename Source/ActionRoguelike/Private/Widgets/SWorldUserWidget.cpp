// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldUserWidget, All, All);

void USWorldUserWidget::SetAttachedActor_Implementation(AActor* Actor)
{
	if (Actor)
	{
		AttachedActor = Actor;
	}
}

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if ((!AttachedActor || !ensure(ParentSizeBox)) && IsValid(this))
	{
		RemoveFromParent();

		UE_LOG(LogWorldUserWidget, Display, TEXT("AttachedActor no longer valid."))
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + VerticalOffset, ScreenPosition))
	{
		const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		check(ViewportScale); // Zero divide protection
		
		ParentSizeBox->SetRenderTranslation(ScreenPosition / ViewportScale);
	}
}
