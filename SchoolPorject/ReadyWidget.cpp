// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyWidget.h"
#include "SchoolPorjectPawn.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UReadyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ReadyButton->OnClicked.AddDynamic(this, &UReadyWidget::ToggleReady);
}

void UReadyWidget::ToggleReady()
{
    ASchoolPorjectPawn* player = Cast<ASchoolPorjectPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    player->OnReady();

    RemoveFromParent();
}