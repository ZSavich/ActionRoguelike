// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(!AttachedActor)
    {
        RemoveFromParent();
        UE_LOG(LogTemp, Warning, TEXT("WorldUserWidget lost AttachedActor. Please fix this!"));
        return;
    }
    
    const auto PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
    const auto AL = AttachedActor->GetActorLocation();
    FVector2D ScreenPosition;
    UGameplayStatics::ProjectWorldToScreen(PC, AL, ScreenPosition);

    const auto ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
    
    ParentSizeBox->SetRenderTranslation(ScreenPosition / ViewportScale + WorldOffset);
}
